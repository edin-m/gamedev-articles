#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>

#include "shader.h"
#include "meshdata.h"
#include "meshloader.h"
#include "camera.h"
#include "textureloader.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "argh.h"

#include "physfs.h"

#include "physfs_assimp_iostream.h"
#include "physfs_assimp_iosystem.h"

#include "physfs_devil.h"

#include "IL/il.h"

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    exit(0);
  }
}

std::string loadTextFileFromPhysFS(const char* fileName) {
  std::string data;
  PHYSFS_File* fp;
  fp = PHYSFS_openRead(fileName);

  char buffer[128];
  PHYSFS_sint64 rc;
  do {
    rc = PHYSFS_readBytes(fp, buffer, sizeof (buffer));
    std::string s(buffer);
    data += s.substr(0, rc);
  } while (!(rc < sizeof(buffer)));

  return data;
}

int main(int argc, char** argv) {
  argh::parser cmdl;
  cmdl.parse(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

  bool use_indices = true;

  if (cmdl["--use-indices"]) {
    use_indices = true;
  }

  if (cmdl["--no-use-indices"]) {
    use_indices = false;
  }

  LOG(INFO) << "Using indices: " << use_indices;

  GLFWwindow* window = nullptr;

  if (!glfwInit()) {
    fprintf( stderr, "ERROR: could not start GLFW3\n" );
    return 1;
  }

#if APPLE
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
  int majorVersion;
  int minorVersion;
  /* get version info */
  renderer = glGetString( GL_RENDERER ); /* get renderer string */
  version = glGetString( GL_VERSION );	 /* version as a string */
  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);	 /* version as a string */
  glGetIntegerv( GL_MINOR_VERSION, &minorVersion);	 /* version as a string */
  printf( "Renderer: %s\n", renderer );
  printf( "OpenGL version supported %s\n", version );
  printf( "OpenGL version supported %d.%d\n", majorVersion, minorVersion );

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int width = 800;
  int height = 600;

  // GL state variables
  GLuint vbo;
  GLuint vao;
  GLuint ebo;

  PHYSFS_init(nullptr);
  int res = PHYSFS_mount("data/physfs-102-data.zip", nullptr, 1);
  LOG(INFO) << "Success: " << (res > 0);

  // load vert shader from physfs
  std::string vertexShader = loadTextFileFromPhysFS("/shaders/assimp-101.vert");
  std::string fragmentShader = loadTextFileFromPhysFS("/shaders/assimp-101.frag");

  k::Shader shader;
  shader.loadShader(vertexShader, fragmentShader);

  // load texture
  k::TextureLoader textureLoader;
  physfs_devil::rebindILToPhysFS();
  k::GLTexture tex;
  tex = textureLoader.loadTexture("/images/texture.png");
  physfs_devil::resetIL();

  // load scene with assimp
  const char* sFilePath = "/objects/suzanne.obj";
  Assimp::Importer importer;
  PhysFS_Assimp_IOSystem* physfs_iosystem = new PhysFS_Assimp_IOSystem();
  importer.SetIOHandler(physfs_iosystem);
  const aiScene* scene = importer.ReadFile(sFilePath,
                                           aiProcess_CalcTangentSpace       |
                                           aiProcess_Triangulate            |
                                           aiProcess_JoinIdenticalVertices  |
                                           aiProcess_SortByPType);
  if (!scene) {
    LOG(ERROR) << "Couldn't load model " << "Error Importing Asset";
    return -1;
  }

  typedef struct {
    aiVector3D vec;
    aiVector2D uv;
    aiVector3D normal;
  } VertexData;

  // we'll assume only one model
  aiMesh* mesh = scene->mMeshes[0];

  int totalNumVertices = mesh->mNumFaces * 3;

  // register vertex array
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // register vertices buffer
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(VertexData) * totalNumVertices,
               nullptr,
               GL_STATIC_DRAW);

  // get pointer to the struct
  VertexData* vertexData = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY_ARB);

  if (!use_indices) {
    // populate buffer - for (drawArrays) use
    for (int i = 0; i < mesh->mNumFaces; i++) {
      aiFace& face = mesh->mFaces[i];
      for (int k = 0; k < 3; k++) {
        aiVector3D pos = mesh->mVertices[face.mIndices[k]];
        aiVector3D uv3 = mesh->mTextureCoords[0][face.mIndices[k]];
        aiVector2D uv = aiVector2D(uv3.x, uv3.y);
        aiVector3D normal = mesh->mNormals[face.mIndices[k]];
        int idx = i * 3 + k;
        vertexData[idx].vec = pos;
        vertexData[idx].uv = uv;
        vertexData[idx].normal = normal;
      }
    }
  } else {
    // populate - for indices (drawElements) use
    for (int i = 0; i < mesh->mNumVertices; i++) {
      aiVector3D pos = mesh->mVertices[i];
      aiVector3D uv3 = mesh->mTextureCoords[0][i];
      aiVector2D uv = aiVector2D(uv3.x, uv3.y);
      aiVector3D normal = mesh->mNormals[i];
      vertexData[i].vec = pos;
      vertexData[i].uv = uv;
      vertexData[i].normal = normal;
    }
  }
  glUnmapBufferARB(GL_ARRAY_BUFFER);

  if (use_indices) {
    // fill element array
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);
    GLuint* indices = (GLuint*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
      for (GLuint j = 0; j < 3; j++) {
        *indices++ = mesh->mFaces[i].mIndices[j];
      }
    }
    glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  shader.use();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(aiVector3D) + sizeof(aiVector2D), 0);

  // Texture coordinates
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                        2 * sizeof(aiVector3D) + sizeof(aiVector2D),
                        (void*) sizeof(aiVector3D));

  // Normal vectors
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(aiVector3D) + sizeof(aiVector2D),
                        (void*) (sizeof(aiVector3D) + sizeof(aiVector2D)));

  shader.use();

  // load camera
  k::Camera camera(width, height);
  camera.move(glm::vec3(-0.13f, 1.11f, 2.11f));

  glm::mat4 mvp = camera.matrix();

  GLuint MatrixID = shader.uniform("mvp");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(mvp));

  glBindVertexArray(0);

  glm::mat4 cammat = camera.matrix();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.texture);

    // set texture uniform
    glUniform1i(tex.textureLocation, 0);

    mvp = cammat * glm::mat4(1.0);
    glBindVertexArray(vao);
    glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    // we must bind this
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if (use_indices) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glDrawElements(GL_TRIANGLES, totalNumVertices, GL_UNSIGNED_INT, (void*)0);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, totalNumVertices);
    }
    glBindVertexArray(0);
    shader.unuse();

    // rotate camera
    cammat = glm::rotate(cammat, glm::radians(1.0f), glm::vec3(0, 1, 0));

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  textureLoader.cleanup(tex);
  PHYSFS_deinit();
  return 0;
}
