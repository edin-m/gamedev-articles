#include "meshgraphics.h"

#include <iostream>

#include "glcheck.h"

namespace k {

MeshGraphics::MeshGraphics() {

}

void MeshGraphics::loadMeshData(MeshData& meshData, LoadedMeshData& loadedMeshData) {
  GLuint* elements = loadedMeshData.elements;

  GLMeshData& glMeshData = meshData.data();

  glMeshData.lenElements = loadedMeshData.lenElements;

  glGenVertexArrays(1, &glMeshData.vao);
  glBindVertexArray(glMeshData.vao);

  glGenBuffers(1, &glMeshData.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMeshData.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLuint) * loadedMeshData.lenElements, elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void MeshGraphics::loadVerticesToShader(MeshData& meshData, const std::string& name) {
  GLMeshData& glMeshData = meshData.data();
  Shader& shader = meshData.shader();

  GLint vPos = shader.attribute(name);

  glBindVertexArray(glMeshData.vao);
  shader.use();
  glBindBuffer(GL_ARRAY_BUFFER, glMeshData.vbo);
  glEnableVertexAttribArray(vPos);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glDisableVertexAttribArray(vPos);
  glBindVertexArray(0);
}

GLuint MeshGraphics::loadVec4ToShader(
    MeshData& meshData,
    const std::string& shaderParamName,
    std::vector<glm::vec4>& data,
    GLint drawType
) {
  GL__(glBindVertexArray(meshData.data().vao));
  GLuint buffer;
  GL__(glGenBuffers(1, &buffer));
  GL__(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GL__(glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * data.size(), &data[0], drawType));

  GLuint pos = meshData.shader().attribute(shaderParamName);
  GL__(glEnableVertexAttribArray(pos));
  GL__(glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0));
  GL__(glDisableVertexAttribArray(pos));

  GL__(glBindVertexArray(0));

  return buffer;
}

GLuint MeshGraphics::loadFloatBufferToShader(
    MeshData& meshData,
    const std::string& shaderParamName,
    int lenItems,
    int perVertexCount,
    GLfloat* data)
{
  GL__(glBindVertexArray(meshData.data().vao));
  GLuint buffer;
  GL__(glGenBuffers(1, &buffer));
  GL__(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GL__(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * lenItems, data, GL_STATIC_DRAW));

  GLuint pos = meshData.shader().attribute(shaderParamName);
  GL__(glEnableVertexAttribArray(pos));
  GL__(glVertexAttribPointer(pos, perVertexCount, GL_FLOAT, GL_FALSE, 0, (void*) 0));

  GL__(glBindVertexArray(0));

  return buffer;
}

GLuint MeshGraphics::loadTexelToShader(MeshData& meshData, LoadedMeshData& loadedData, const std::string& paramName) {
  return loadFloatBufferToShader(meshData, paramName, loadedData.lenTexelData,
                                 loadedData.numOfTexelsPerElement, loadedData.texelData);
}

GLuint MeshGraphics::loadVerticesToShader(MeshData& meshData, LoadedMeshData& loadedData, const std::string& paramName) {
  return loadFloatBufferToShader(meshData, paramName, loadedData.lenVertexBufferData,
                                 loadedData.numOfVerticesPerElement, loadedData.vertexBufferData);
}


void MeshGraphics::renderMesh(MeshData& meshData) {
  GLMeshData& data = meshData.data();
  Shader& shader = meshData.shader();
  shader.use();
  glBindVertexArray(data.vao);
  glDrawArrays(GL_TRIANGLES, 0, data.lenElements);
  shader.unuse();
}

void MeshGraphics::cleanupMeshData(MeshData& meshData) {
  meshData.shader().deleteShader();
  GLMeshData& glMeshData = meshData.data();
  glDeleteBuffers(1, &glMeshData.ebo);
  glDeleteBuffers(1, &glMeshData.vbo);
  glDeleteVertexArrays(1, &glMeshData.vao);

}

}
