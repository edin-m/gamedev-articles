
#include <stdio.h>
#include <iostream>

#include <functional>

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "bvh.h"

#include "camera.h"

#include "meshdata.h"
#include "meshgraphics.h"
#include "meshloader.h"
#include "textureloader.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "glcheck.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))



static GLuint vboVert, vboIndices;
static GLuint vao;


float rotationAngle = 0.0f;

glm::mat4 model = glm::mat4(1.0);

static short bvh_elements = 0;

bool gogogo = true;

k::Bvh* bvh;
int frame = 0, frameChange = 1;

void tmpProcess(k::JOINT* joint,
                std::vector<glm::vec4>& vertices,
                std::vector<GLshort>& indices,
                GLshort parentIndex = 0)
{
    glm::vec4 translatedVertex = joint->matrix[3];

    vertices.push_back(translatedVertex);

    GLshort myindex = vertices.size() - 1;

    if( parentIndex != myindex )
    {
        indices.push_back(parentIndex);
        indices.push_back(myindex);
    }

    for(auto& child : joint->children) {
        tmpProcess(child, vertices, indices, myindex);
    }
}

void update() {

  if (frameChange) {
    frame = frame + frameChange;
  } else {
    return;
  }

  int frameto = frame % bvh->getNumFrames();
  std::cout << "move to " << frameto << std::endl;
  bvh->moveTo(frameto);

  std::vector<glm::vec4> vertices;
  std::vector<GLshort> bvhindices;

  tmpProcess((k::JOINT*)bvh->getRootJoint(), vertices, bvhindices);



//    for (int i = 0; i < vertices.size(); i++) {
//      std::cout << vertices[i].x << " "
//                   << vertices[i].y << " "
//                      << vertices[i].z << " "
//                << std::endl;
//    }

  glBindBuffer(GL_ARRAY_BUFFER, vboVert);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    exit(0);
  }
  if (key == GLFW_KEY_J && action == GLFW_PRESS) {
    frameChange++;
  }
  if (key == GLFW_KEY_K && action == GLFW_PRESS) {
    frameChange--;
  }
}

int main(int argc, char* argv[]) {
  START_EASYLOGGINGPP(argc, argv);
  LOG(INFO) << "Test SLOG INFO";

  GLFWwindow* window = nullptr;

  if (!glfwInit()) {
    LOG(ERROR) << "ERROR: could not start GLFW3";
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
    LOG(ERROR) << "ERROR: could not open window with GLFW3";
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
  LOG(INFO) << "Renderer: " << renderer;
  LOG(INFO) << "OpenGL version supported" << version;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glLineWidth(3.0);
  glPointSize(5.0);

  int width = 512;
  int height = 512;











  glm::vec3 bvh_scale = glm::vec3(0.25, 0.25, 0.25);

    k::Shader bvhShader;
    bvhShader.loadShader("shader1");
    bvh = new k::Bvh;
    bvh->load("../data/bvh/0008_ChaCha001.bvh");
    bvh->testOutput();
    bvh->printJoint(bvh->getRootJoint());
    bvh->moveTo(frame++);


    std::vector<glm::vec4> vertices;
    std::vector<GLshort> bvhindices;


    tmpProcess((k::JOINT*)bvh->getRootJoint(), vertices, bvhindices);

    bvh_elements = bvhindices.size();

    glGenBuffers(1, &vboVert);
    glBindBuffer(GL_ARRAY_BUFFER, vboVert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bvhindices[0]) * bvhindices.size(), &bvhindices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    bvhShader.use();

    GLint sLocPosition = bvhShader.attribute("vPosition");

    glBindBuffer(GL_ARRAY_BUFFER, vboVert);
    glEnableVertexAttribArray(sLocPosition);
    glVertexAttribPointer(sLocPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


  for (int i = 0; i < vertices.size(); i++) {
    std::cout << "vertices "
              << vertices[i].x << " "
              << vertices[i].y << " "
              << vertices[i].z << " "
              << std::endl;
  }




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

      texture.textureLocation = glGetUniformLocation(shader.getProgram(), "tex");
      shader.unuse();


      camera.move(glm::vec3(-28.13f, 30.11f, 20.11f));

      while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);








        glm::mat4 mvp = camera.matrix();// * glm::translate(model, glm::vec3(15, 0, 0));

        bvhShader.use();

        glm::mat4 _model = glm::scale(model, bvh_scale);

        mvp = camera.matrix() * _model;
        glUniformMatrix4fv(bvhShader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

        update();
        glBindVertexArray(vao);

        glPointSize(5.0);
        glDrawElements(GL_LINES, bvh_elements, GL_UNSIGNED_SHORT, (GLvoid*) 0);
        glLineWidth(3.0);
        glDrawElements(GL_POINTS, bvh_elements, GL_UNSIGNED_SHORT, (GLvoid*) 0);

        glBindVertexArray(0);

        bvhShader.unuse();















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




        glfwSwapBuffers( window );
        glfwPollEvents();
      }

      meshGraphics.cleanupMeshData(meshData);

      return 0;
    }
