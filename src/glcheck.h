#ifndef GLBASIC_H
#define GLBASIC_H

#include <iostream>

#include "GL/glew.h"

namespace k {

const char* GetOpenGLErrorString(int errId) {
  switch (errId) {
    case GL_INVALID_VALUE: return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_INVALID_ENUM: return "Invalid enum";
    case GL_STACK_OVERFLOW: return "Stack overflow";
    case GL_STACK_UNDERFLOW: return "Stack underflow";
    case GL_OUT_OF_MEMORY: return "Out of memory";
    default: return "Not found";
  }
}

inline void CheckOpenGLError(const char* stmt, const char* fname, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cout << "OpenGL error %08x, (%s) at %s:%i - for %s"
              << err
              << GetOpenGLErrorString(err)
              << fname
              << line
              << stmt;
  }
}

}

#ifndef GL_CHECK
#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
stmt; \
k::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0);
#else
#define GL_CHECK(stmt) stmt
#endif
#endif

#endif // GLBASIC_H
