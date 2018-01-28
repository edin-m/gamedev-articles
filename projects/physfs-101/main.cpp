
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

#include "physfs.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "argh.h"

#define BUFFER_OFFSET(offset) ((void *)(offset))

void phys_IL_close(ILHANDLE handle) {
  PHYSFS_close((PHYSFS_File*) handle);
}

ILboolean phys_IL_eof(ILHANDLE handle) {
  return PHYSFS_eof((PHYSFS_File*) handle) == 0 ? IL_FALSE : IL_TRUE;
}

ILint phys_IL_getc(ILHANDLE handle) {
  char c;
  int x = PHYSFS_readBytes((PHYSFS_File*) handle, &c, 1);
  if (x < 1) {
    return IL_EOF;
  }
  return c;
}

ILHANDLE phys_IL_open(ILconst_string file) {
  PHYSFS_File* fp;
  fp = PHYSFS_openRead(file);
  return (ILHANDLE) fp;
}

ILint phys_IL_read(void* Buffer, ILuint Size, ILuint Number, ILHANDLE Handle) {
  return ILint(PHYSFS_readBytes((PHYSFS_File*) Handle, Buffer, Size * Number));
}

ILint phys_IL_seek(ILHANDLE Handle, ILint Offset, ILint Mode) {
  if (Mode == IL_SEEK_END) {
    PHYSFS_sint64 size = PHYSFS_fileLength((PHYSFS_File*) Handle);
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, (PHYSFS_uint64) size);
  } else if (Mode == IL_SEEK_SET) {
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, (PHYSFS_uint64) Offset);
  } else if (Mode == IL_SEEK_CUR) {
    PHYSFS_sint64 curr = PHYSFS_tell((PHYSFS_File*) Handle);
    return (ILint) PHYSFS_seek((PHYSFS_File*) Handle, curr + ((PHYSFS_uint64) Offset));
  }
  return -1;
}

ILint phys_IL_tell(ILHANDLE handle) {
  return (ILint) PHYSFS_tell((PHYSFS_File*) handle);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    exit(0);
  }
}

int main(int argc, char** argv) {
  argh::parser cmdl;
  cmdl.parse(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

  bool rebind_il = false;

  if (cmdl["--help"]) {
    LOG(INFO) << "Usage:";
    LOG(INFO) << "--rebind-il - rebinds IL read methods, so it can read directly from PhysFS";
    LOG(INFO) << "--no-rebind-il - loads from PhysFS into the memory first then to IL";
    return 0;
  }

  if (cmdl["--rebind-il"]) {
    rebind_il = true;
  }

  if (cmdl["--no-rebind-il"]) {
    rebind_il = false;
  }

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

  PHYSFS_init(nullptr);
  int res = PHYSFS_mount("data/physfs-101-data.zip", nullptr, 1);
  LOG(INFO) << "Success: " << (res > 0);

  bool exists = PHYSFS_exists("/images/texture.png");
  LOG(INFO) << "exists " << exists;

  exists = PHYSFS_exists("/shaders/texture-101.vert");
  LOG(INFO) << "exists " << exists;
  {
    char **rc = PHYSFS_enumerateFiles("/");
    char **i;
    LOG(INFO) << "enumerating";
    for (i = rc; *i != NULL; i++) {
      LOG(INFO) << "file: " << (*i);
    }
  }

  std::string vertexShader, fragmentShader;

  // load vert shader from physfs
  {
    PHYSFS_File* fp;
    fp = PHYSFS_openRead("/shaders/texture-101.vert");

    char buffer[128];
    PHYSFS_sint64 rc;
    PHYSFS_sint64 i;
    do {
      rc = PHYSFS_readBytes(fp, buffer, sizeof (buffer));
      std::string s(buffer);
      vertexShader += s.substr(0, rc);
    } while (!(rc < sizeof(buffer)));
  }
  // load fragment shader from physfs
  {
    PHYSFS_File* fp;
    fp = PHYSFS_openRead("/shaders/texture-101.frag");

    char buffer[128];
    PHYSFS_sint64 rc;
    PHYSFS_sint64 i;
    do {
      rc = PHYSFS_readBytes(fp, buffer, sizeof (buffer));
      std::string s(buffer);
      fragmentShader += s.substr(0, rc);
    } while (!(rc < sizeof(buffer)));
  }

  k::TextureLoader textureLoader;
  k::GLTexture texture;

  // if we're not rebinding IL methods, then manually load from physfs to memory and from memory to IL
  if (!rebind_il) {
    LOG(INFO) << "Using memory copy to load image to IL";
    std::vector<char> data;
    PHYSFS_File* fp;
    fp = PHYSFS_openRead("/images/texture.png");

    char buffer[128];
    PHYSFS_sint64 rc;
    do {
      rc = PHYSFS_readBytes(fp, buffer, sizeof (buffer));
      for (int i = 0; i < rc; i++) {
        data.push_back(buffer[i]);
      }
    } while (!(rc < sizeof(buffer)));

    ILuint image = ilGenImage();
    ilBindImage(image);
    ilLoadL(IL_PNG, &data[0], sizeof(data[0]) * data.size());
    texture = textureLoader.loadILTexture(image);
  }

  if (rebind_il) {
    LOG(INFO) << "Using PhysFS-overriden methods so IL can load directly from PhysFS";
    ilSetRead((fOpenRProc) &phys_IL_open, (fCloseRProc) &phys_IL_close,
              (fEofProc) &phys_IL_eof, (fGetcProc) &phys_IL_getc,
              (fReadProc) &phys_IL_read, (fSeekRProc) &phys_IL_seek,
              (fTellRProc) &phys_IL_tell);
    texture = textureLoader.loadTexture("/images/texture.png");
    ilResetRead();
  }

  k::Shader loadedShader;
  loadedShader.loadShader(vertexShader, fragmentShader);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int width = 512;
  int height = 512;

  k::Camera camera(width, height);
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 mvp = camera.matrix() * Model;

  k::MeshData meshData(loadedShader);
  k::MeshGraphics meshGraphics;

  k::Shader& shader = meshData.shader();
  // load mesh
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

  PHYSFS_deinit();

  return 0;
}
