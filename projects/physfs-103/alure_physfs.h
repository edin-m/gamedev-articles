#ifndef ALURE_PHYSFS_H
#define ALURE_PHYSFS_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <limits>
#include <thread>
#include <chrono>

#include "physfs.h"

#include "alure2.h"

namespace alure_physfs {

// Inherit from std::streambuf to handle custom I/O (PhysFS for this example)
class PhysFSBuf final : public std::streambuf {
  alure::Array<char_type, 4096> mBuffer;
  PHYSFS_File* mFile { nullptr };

  int_type underflow() override {
    if (mFile && gptr() == egptr()) {
      // Read in the next chunk of data, and set the read pointers on
      // success
      PHYSFS_sint64 got = PHYSFS_readBytes(mFile,
                                           mBuffer.data(), sizeof(char_type) * mBuffer.size()
                                           );
      if (got != -1) {
        setg(mBuffer.data(), mBuffer.data(), mBuffer.data() + got);
      }
    }
    if (gptr() == egptr()) {
      return traits_type::eof();
    }
    return traits_type::to_int_type(*gptr());
  }

  pos_type seekoff(off_type offset, std::ios_base::seekdir whence,
                   std::ios_base::openmode mode) override
  {
    if (!mFile || (mode & std::ios_base::out) || !(mode & std::ios_base::in)) {
      return traits_type::eof();
    }

    // PhysFS only seeks using absolute offsets, so we have to convert cur-
    // and end-relative offsets.
    PHYSFS_sint64 fpos;
    switch(whence) {
      case std::ios_base::beg:
        break;

      case std::ios_base::cur:
        // Need to offset for the file offset being at egptr() while
        // the requested offset is relative to gptr().
        offset -= off_type(egptr()-gptr());
        if((fpos=PHYSFS_tell(mFile)) == -1)
          return traits_type::eof();
        // If the offset remains in the current buffer range, just
        // update the pointer.
        if(offset < 0 && -offset <= off_type(egptr() - eback())) {
          setg(eback(), egptr()+offset, egptr());
          return fpos + offset;
        }
        offset += fpos;
        break;

      case std::ios_base::end:
        if((fpos=PHYSFS_fileLength(mFile)) == -1) {
          return traits_type::eof();
        }
        offset += fpos;
        break;

      default:
        return traits_type::eof();
    }

    if (offset < 0) {
      return traits_type::eof();
    }
    if (PHYSFS_seek(mFile, offset) == 0) {
      // HACK: Workaround a bug in PhysFS. Certain archive types error
      // when trying to seek to the end of the file. So if seeking to the
      // end of the file fails, instead try seeking to the last byte and
      // read it.
      if(offset != PHYSFS_fileLength(mFile))
        return traits_type::eof();
      if(PHYSFS_seek(mFile, offset-1) == 0)
        return traits_type::eof();
      PHYSFS_readBytes(mFile, mBuffer.data(), 1 * 1);
    }
    // Clear read pointers so underflow() gets called on the next read
    // attempt.
    setg(nullptr, nullptr, nullptr);
    return offset;
  }

  pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override {
    // Simplified version of seekoff
    if (!mFile || (mode & std::ios_base::out) || !(mode & std::ios_base::in)) {
      return traits_type::eof();
    }

    if (PHYSFS_seek(mFile, pos) == 0) {
      return traits_type::eof();
    }
    setg(nullptr, nullptr, nullptr);
    return pos;
  }

public:
  bool open(const char* filename) noexcept {
    mFile = PHYSFS_openRead(filename);
    if (!mFile) {
      return false;
    }
    return true;
  }

  PhysFSBuf() = default;

  ~PhysFSBuf() override {
    PHYSFS_close(mFile);
    mFile = nullptr;
  }
};

// Inherit from std::istream to use our custom streambuf
class Stream final : public std::istream {
  PhysFSBuf mStreamBuf;

public:
  Stream(const char *filename) : std::istream(nullptr) {
    init(&mStreamBuf);

    // Set the failbit if the file failed to open.
    if (!mStreamBuf.open(filename)) {
      clear(failbit);
    }
  }
};

// Inherit from alure::FileIOFactory to use our custom istream
class FileFactory final : public alure::FileIOFactory {
public:
  alure::UniquePtr<std::istream> openFile(const alure::String &name) noexcept override
  {
    std::cout << "openFile() " << name << std::endl;
    auto stream = alure::MakeUnique<Stream>(name.c_str());
    if(stream->fail()) stream = nullptr;
    return std::move(stream);
  }

  // A PhysFS-specific function to mount a new path to the virtual directory
  // tree.
  static bool Mount(const char* path, const char* mountPoint = nullptr, int append = 0)
  {
    std::cout << "Adding new file source " << path;
    if (mountPoint) {
      std::cout << " to " << mountPoint;
    }
    std::cout<<"..."<<std::endl;

    if (PHYSFS_mount(path, mountPoint, append) == 0) {
      auto error = PHYSFS_getLastErrorCode();
      std::cerr << "Failed to add " << path << ": "
                << PHYSFS_getErrorByCode(error)
                << std::endl;
      return false;
    }
    return true;
  }

  static void ListDirectory(std::string&& dir)
  {
    char **files = PHYSFS_enumerateFiles(dir.c_str());
    for (int i = 0; files[i]; i++)
    {
      std::string file = dir + files[i];
      PHYSFS_Stat stat;
      PHYSFS_stat(file.c_str(), &stat);
      if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
        ListDirectory(file+"/");
      } else {
        std::cout<<"  "<<file<<"\n";
      }
    }
    PHYSFS_freeList(files);
  }
};

// Helper class+method to print the time with human-readable formatting.
struct PrettyTime {
  alure::Seconds mTime;
};

inline std::ostream &operator<<(std::ostream &os, const PrettyTime &rhs)
{
  using hours = std::chrono::hours;
  using minutes = std::chrono::minutes;
  using seconds = std::chrono::seconds;
  using centiseconds = std::chrono::duration<int64_t, std::ratio<1, 100>>;
  using std::chrono::duration_cast;

  centiseconds t = duration_cast<centiseconds>(rhs.mTime);
  if(t.count() < 0)
  {
    os << '-';
    t *= -1;
  }

  // Only handle up to hour formatting
  if (t >= hours(1)) {
    os << duration_cast<hours>(t).count() << 'h' << std::setfill('0') << std::setw(2)
       << duration_cast<minutes>(t).count() << 'm';
  } else {
    os << duration_cast<minutes>(t).count() << 'm' << std::setfill('0');
    os << std::setw(2) << (duration_cast<seconds>(t).count() % 60) << '.' << std::setw(2)
       << (t.count() % 100) << 's' << std::setw(0) << std::setfill(' ');
  }
  return os;
}

} // namespace

#endif // ALURE_PHYSFS_H
