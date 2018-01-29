#include "physfs_assimp_iostream.h"

PhysFS_Assimp_IOStream::PhysFS_Assimp_IOStream(PHYSFS_File *handle)
  : handle_(handle),
    Assimp::IOStream() {

}

PHYSFS_File* PhysFS_Assimp_IOStream::getHandle() {
  return handle_;
}

size_t PhysFS_Assimp_IOStream::Read(void* pvBuffer, size_t pSize, size_t pCount) {
  return (size_t) PHYSFS_readBytes(handle_, pvBuffer, pSize * pCount);
}

size_t PhysFS_Assimp_IOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
  return (size_t) PHYSFS_writeBytes(handle_, pvBuffer, pSize * pCount);
}

aiReturn PhysFS_Assimp_IOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
  PHYSFS_uint64 offset = (PHYSFS_uint64) pOffset;
  if (pOrigin == aiOrigin_SET) {
    PHYSFS_seek(handle_, offset);
  } else if (pOrigin == aiOrigin_CUR) {
    PHYSFS_sint64 curr = PHYSFS_tell(handle_);
    PHYSFS_seek(handle_, curr + offset);
  } else if (pOrigin == aiOrigin_END) {
    PHYSFS_sint64 size = PHYSFS_fileLength(handle_);
    PHYSFS_seek(handle_, (PHYSFS_uint64) size);
  } else {
    return aiReturn_FAILURE;
  }
  return aiReturn_SUCCESS;
}

size_t PhysFS_Assimp_IOStream::Tell() const {
  return (size_t) PHYSFS_tell(handle_);
}

size_t PhysFS_Assimp_IOStream::FileSize() const {
  return (size_t) PHYSFS_fileLength(handle_);
}

void PhysFS_Assimp_IOStream::Flush() {
  PHYSFS_flush(handle_);
}
