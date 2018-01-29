#include "physfs_assimp_iosystem.h"

#include "physfs_assimp_iostream.h"

PhysFS_Assimp_IOSystem::PhysFS_Assimp_IOSystem()
 : Assimp::IOSystem() {

}

bool PhysFS_Assimp_IOSystem::Exists(const char* pFile) const {
  return PHYSFS_exists(pFile);
}

char PhysFS_Assimp_IOSystem::getOsSeparator() const {
  return '/';
}

Assimp::IOStream* PhysFS_Assimp_IOSystem::Open(const char* pFile, const char* pMode) {
  PHYSFS_File* handle = PHYSFS_openRead(pFile);
  return (Assimp::IOStream*) new PhysFS_Assimp_IOStream(handle);
}

void PhysFS_Assimp_IOSystem::Close(Assimp::IOStream* pFile) {
  PhysFS_Assimp_IOStream* physfs_stream = (PhysFS_Assimp_IOStream*) pFile;
  PHYSFS_close(physfs_stream->getHandle());
  delete physfs_stream;
}
