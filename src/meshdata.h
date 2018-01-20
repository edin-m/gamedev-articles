#ifndef MESHDATA_H
#define MESHDATA_H

#include <string>

#include <GL/glew.h>

#include "shader.h"

namespace k {

struct GLMeshData {
  GLuint vao;
  GLuint ebo;
  GLuint vbo;
  int lenElements;
};

class MeshData {
  Shader shader_;
  GLMeshData glMeshData;

public:
  MeshData(const std::string& shaderName);

  GLMeshData& data() { return glMeshData; }
  Shader& shader() { return shader_; }
};

}

#endif // MESHDATA_H
