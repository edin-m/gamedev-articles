#ifndef PHYSFS_ASSIMP_IOSYSTEM_H
#define PHYSFS_ASSIMP_IOSYSTEM_H

#include "physfs.h"

#include "assimp/IOSystem.hpp"

class PhysFS_Assimp_IOSystem : public Assimp::IOSystem {
public:
  PhysFS_Assimp_IOSystem();
  bool Exists(const char* pFile) const;
  char getOsSeparator() const;
  Assimp::IOStream* Open(const char* pFile, const char* pMode = "rb");
  void Close(Assimp::IOStream* pFile);
};

#endif // PHYSFS_ASSIMP_IOSYSTEM_H
