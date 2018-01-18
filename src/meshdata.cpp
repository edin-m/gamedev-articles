#include "meshdata.h"

namespace k {

MeshData::MeshData(const std::string& shaderName) {
  shader_.loadShader(shaderName);
}

MeshData::~MeshData() {
  shader_.deleteShader();
  glDeleteBuffers(1, &glMeshData.ebo);
  glDeleteBuffers(1, &glMeshData.vbo);
  glDeleteVertexArrays(1, &glMeshData.vao);
}

}
