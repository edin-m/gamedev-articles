#ifndef PHYSFS_ASSIMP_IOSTREAM_H
#define PHYSFS_ASSIMP_IOSTREAM_H

#include "physfs.h"

#include "assimp/IOStream.hpp"

class PhysFS_Assimp_IOStream : public Assimp::IOStream {
  PHYSFS_File* handle_;
public:
  PhysFS_Assimp_IOStream(PHYSFS_File* handle);
  PHYSFS_File* getHandle();
  size_t Read(void* pvBuffer, size_t pSize, size_t pCount);
  size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);
  aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
  size_t Tell() const;
  size_t FileSize() const;
  void Flush();
};

#endif // PHYSFS_ASSIMP_IOSTREAM_H
