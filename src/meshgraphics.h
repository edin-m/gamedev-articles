#ifndef MESHGRAPHICS_H
#define MESHGRAPHICS_H

#include "meshdata.h"
#include "meshloader.h"

namespace k {

class MeshGraphics
{
public:
  MeshGraphics();

  void loadMeshData(MeshData& meshData, LoadedMeshData& loadedMeshData);
  void loadVerticesToShader(MeshData& meshData, const std::string& name);
  void renderMesh(MeshData& meshData);
};

}

#endif // MESHGRAPHICS_H
