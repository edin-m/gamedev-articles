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
  GLuint loadFloatBufferToShader(
      MeshData& meshData,
      const std::string& shaderParamName,
      int lenItems,
      int perVertexCount,
      GLfloat* data
  );
  GLuint loadTexelToShader(MeshData& meshData, LoadedMeshData& loadedData, const std::string& paramName);
  GLuint loadVerticesToShader(MeshData& meshData, LoadedMeshData& loadedData, const std::string& paramName);
  void renderMesh(MeshData& meshData);
  void cleanupMeshData(MeshData& meshData);
};

}

#endif // MESHGRAPHICS_H
