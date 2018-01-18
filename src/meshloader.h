#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "meshdata.h"

namespace k {

struct LoadedMeshData {
  GLushort* elements;
  GLfloat* vertexBufferData;
  int lenVertexBufferData;
  int lenElements;
};

class MeshLoader
{
public:
  MeshLoader();
};

}

#endif // MESHLOADER_H
