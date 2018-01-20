#include "textureloader.h"

#include "GL/glew.h"

#include "glcheck.h"

#include "IL/il.h"

namespace k {

TextureLoader::TextureLoader()
{
  ilInit();
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GLTexture TextureLoader::loadTexture(const std::string& filename) {
  GLTexture glTexture;

  GL_CHECK(glGenBuffers(1, &glTexture.texture));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, glTexture.texture));

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

  ILuint image = ilGenImage();
  ilBindImage(image);
  ilLoadImage(filename.c_str());
  ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

  glTexture.width = ilGetInteger(IL_IMAGE_WIDTH);
  glTexture.height = ilGetInteger(IL_IMAGE_HEIGHT);
  glTexture.format = ilGetInteger(IL_IMAGE_FORMAT);
  glTexture.type = ilGetInteger(IL_IMAGE_TYPE);

  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, (GLint) glTexture.format, glTexture.width,
                        glTexture.height, 0, (GLenum) glTexture.format, glTexture.type,
                        ilGetData()));

  ilDeleteImage(image);

  return glTexture;
}

void TextureLoader::cleanup(GLTexture& glTexture) {
  glDeleteBuffers(1, &glTexture.texture);
}

}

