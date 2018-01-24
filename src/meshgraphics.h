#ifndef MESHGRAPHICS_H
#define MESHGRAPHICS_H

#include <vector>

#include "GL/glew.h"

#include "glm/glm.hpp"

#include "meshdata.h"
#include "meshloader.h"

namespace k {

class MeshGraphics
{
public:
  MeshGraphics();

  void loadMeshData(MeshData& meshData, LoadedMeshData& loadedMeshData);
  void loadVerticesToShader(MeshData& meshData, const std::string& name);
  GLuint loadVec4ToShader(
      MeshData& meshData,
      const std::string& shaderParamName,
      std::vector<glm::vec4>& data,
      GLint drawType = GL_STATIC_DRAW
  );
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
