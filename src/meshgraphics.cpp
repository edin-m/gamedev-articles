#include "meshgraphics.h"

#include <iostream>

namespace k {

MeshGraphics::MeshGraphics() {

}

void MeshGraphics::loadMeshData(MeshData& meshData, LoadedMeshData& loadedMeshData) {
  GLushort* elements = loadedMeshData.elements;
  GLfloat* vertices = loadedMeshData.vertexBufferData;

  GLMeshData& glMeshData = meshData.data();

  glMeshData.lenElements = loadedMeshData.lenElements;

  glGenVertexArrays(1, &glMeshData.vao);
  glBindVertexArray(glMeshData.vao);

  glGenBuffers(1, &glMeshData.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMeshData.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLushort) * loadedMeshData.lenElements, elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenBuffers(1, &glMeshData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, glMeshData.vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) * loadedMeshData.lenVertexBufferData, vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void MeshGraphics::loadVerticesToShader(MeshData& meshData, const std::string& name) {
  GLMeshData& glMeshData = meshData.data();
  Shader& shader = meshData.shader();

  GLint vPos = shader.attribute(name);

  glBindVertexArray(glMeshData.vao);
  shader.use();
  glEnableVertexAttribArray(vPos);
  glBindBuffer(GL_ARRAY_BUFFER, glMeshData.vbo);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glBindVertexArray(0);
}

void MeshGraphics::renderMesh(MeshData& meshData) {
  GLMeshData& data = meshData.data();
  Shader& shader = meshData.shader();
  shader.use();
  glBindVertexArray(data.vao);
  glDrawArrays(GL_TRIANGLES, 0, data.lenElements);
  shader.unuse();
}

}
