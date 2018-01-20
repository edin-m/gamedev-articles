
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "shader.h"

#include "camera.h"

#include "IL/il.h"

#include "glcheck.h"

#include "meshdata.h"
#include "meshloader.h"
#include "meshgraphics.h"
#include "textureloader.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    exit(0);
  }
}

int main() {
  GLFWwindow* window = nullptr;

  if (!glfwInit()) {
    fprintf( stderr, "ERROR: could not start GLFW3\n" );
    return 1;
  }

#if APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 480, "Hello world", nullptr, nullptr);

  if (!window) {
    fprintf( stderr, "ERROR: could not open window with GLFW3\n" );
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, key_callback);

  const GLubyte* renderer;
  const GLubyte* version;
  /* get version info */
  renderer = glGetString( GL_RENDERER ); /* get renderer string */
  version = glGetString( GL_VERSION );	 /* version as a string */
  printf( "Renderer: %s\n", renderer );
  printf( "OpenGL version supported %s\n", version );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int width = 512;
  int height = 512;

  k::Camera camera(width, height);
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = camera.matrix() * Model;

  k::TextureLoader textureLoader;
  k::MeshData meshData("shader4");
  k::MeshGraphics meshGraphics;

  k::Shader& shader = meshData.shader();
  {
    GLfloat* texdata = new GLfloat[8] {
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f
    };

    // data for a fullscreen quad (this time with texture coords)
    GLfloat* vertexData = new GLfloat[9] {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    GLuint* indexData = new GLuint[3] {
        0, 1, 2,
    };

    // load meshdata (vertices and indicies)
    k::LoadedMeshData loadedData;
    loadedData.elements = indexData;
    loadedData.lenElements = 3;
    loadedData.vertexBufferData = vertexData;
    loadedData.lenVertexBufferData = 9;
    loadedData.texelData = texdata;
    loadedData.lenTexelData = 8;

    meshGraphics.loadMeshData(meshData, loadedData);

    meshGraphics.loadTexelToShader(meshData, loadedData, "uvPos");
    meshGraphics.loadVerticesToShader(meshData, loadedData, "vPosition");

    delete[] indexData;
    delete[] vertexData;
  }

  k::GLTexture texture = textureLoader.loadTexture("../data/images/texture.png");

  shader.use();
  GLuint MatrixID = shader.uniform("mvp");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));
  shader.unuse();

  texture.textureLocation = glGetUniformLocation(shader.getProgram(), "tex");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the shader program
    shader.use();

    // bind texture to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.texture);

    // set texture uniform
    glUniform1i(texture.textureLocation, 0);

    GLuint MatrixID = shader.uniform("mvp");
    glm::mat4 mvp_ = camera.matrix() * glm::mat4(1.0f);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp_));

    meshGraphics.renderMesh(meshData);
    shader.unuse();

    camera.move(glm::vec3(-0.01f, 0.0f, 0.0f));

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  meshGraphics.cleanupMeshData(meshData);
  textureLoader.cleanup(texture);

  return 0;
}
