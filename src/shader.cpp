#include "shader.h"

#include <iostream>

#include "glcheck.h"

namespace k {

Shader::Shader()
  : program(0) {

}

Shader::~Shader() {
  deleteShader();
}

GLuint Shader::loadShader(const std::string& path) {
  deleteShader();
  this->name = name;

  const std::string fragPath = path + ".frag";
  const std::string vertPath = path + ".vert";

  GLuint fragid = loadShader(GL_FRAGMENT_SHADER, fragPath);
  GLuint vertid = loadShader(GL_VERTEX_SHADER, vertPath);

  if (fragid == 0) {
    std::cout << "Fragment shader failed!";
    // TODO: throw exception
  }

  if( vertid == 0 ) {
    std::cout << "Vertex shader failed!";
    // TODO: throw exception
  }

  if( fragid == 0 || vertid == 0 ) {
    // TODO: throw exception
    return 0;
  }

  std::vector<GLuint> shaders;
  shaders.push_back(fragid);
  shaders.push_back(vertid);

  program = createProgram(shaders);

  if( program > 0 ) {
    //        klog.i("shader") << "Shader program successfully created.";
  } else {
    //        klog.e("shader") << "Shader program creation failed!";
  }

  return program;
}

bool Shader::deleteShader() {
  if (program > 0) {
    GL_CHECK(glDeleteProgram(program));
  }
  return true;
}

void Shader::use() {
  GL_CHECK(glUseProgram(program));
}

void Shader::unuse() {
  GL_CHECK(glUseProgram(0));
}

GLuint Shader::getProgram() const {
  return program;
}

GLuint Shader::loadShader(GLuint type, const std::string& path) {
  std::fstream in;
  in.open(path.c_str(), std::ios_base::in);

  std::string data;
  if (in.is_open()) {
    std::string line;
    while (in.good()) {
      std::getline(in, line);
      data += (line + "\n");
    }
    in.close();
  } else {
    //        klog.e("shader") << "File not opened: " << path;
    in.close();
    return 0;
  }

  //    klog.i("shader") << "Shader read from" << path;
  return uploadShader(type, data.c_str());
}

GLuint Shader::uploadShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  GL_CHECK(glShaderSource(shader, 1, (const GLchar**)&source, NULL));
  GL_CHECK(glCompileShader(shader));

  return shader;
}

GLuint Shader::createProgram(const std::vector<GLuint>& shaders) {
  GLuint _program = glCreateProgram();
  for(std::vector<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
    GLuint shader = (GLuint) *it;
    GL_CHECK(glAttachShader(_program, shader));
  }

  GL_CHECK(glLinkProgram(_program));

  for(std::vector<GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
    GLuint shader = (GLuint) *it;
    GL_CHECK(glDetachShader(_program, shader));
    GL_CHECK(glDeleteShader(shader));
  }

  return _program;
}

GLint Shader::addAttribute(const std::string& attr) {
  const GLchar* _attr = attr.c_str();

  if (program != 0) {
    if (attributes.count(_attr) > 0) {
      return attributes[_attr];
    }

    GLint attrloc = GL_CHECK(glGetAttribLocation(program, _attr));
    if (attrloc == -1) {
      std::cout << "Could not find attribute location " << attr << std::endl;
    }
    attributes[_attr] = attrloc;
    return attrloc;
  }
  return -1;
}

GLint Shader::addUniform(const std::string& uniform) {
  const GLchar* _uniform = uniform.c_str();

  if (program != 0) {
    if (uniforms.count(_uniform) > 0) {
      return uniforms[_uniform];
    }

    GLint uniformloc = GL_CHECK(glGetUniformLocation(program, _uniform));
    if (uniformloc == -1) {
      std::cout << "Could not find uniform location " << uniform << std::endl;
    }
    uniforms[(const char*)_uniform] = uniformloc;
    return uniformloc;
  }
  return -1;
}

GLint Shader::attribute(const std::string& attr) {
  return addAttribute(attr);
}

GLint Shader::uniform(const std::string& uniform) {
  return addUniform(uniform);
}

}
