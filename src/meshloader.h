#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "meshdata.h"

namespace k {

struct LoadedMeshData {
  GLuint* elements;
  GLfloat* vertexBufferData;
  GLfloat* texelData;
  int lenVertexBufferData;
  int lenElements;
  int lenTexelData;
  constexpr static const int numOfVerticesPerElement = 3;
  constexpr static const int numOfTexelsPerElement = 2;
};

class MeshLoader
{
public:
  MeshLoader();
};

}

#endif // MESHLOADER_H
