#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>

#include "GL/glew.h"

namespace k {

/**
 * @brief The GLTexture struct
 */
struct GLTexture {
  GLuint textureLocation;
  GLuint texture;
  int width;
  int height;
  int format;
  GLenum type;
};

/**
 * @brief The TextureLoader class
 */
class TextureLoader
{
public:
  TextureLoader();
  GLTexture loadTexture(const std::string& filename);
  void cleanup(GLTexture& glTexture);
};

}

#endif // TEXTURELOADER_H
