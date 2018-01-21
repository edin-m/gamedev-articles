#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace k {

class Camera
{
  constexpr static const float ZNEAR = 0.1f;
  constexpr static const float ZFAR = 100.0f;

  glm::vec3 location;
  glm::vec3 lookat;
  glm::vec3 upvec;

  glm::mat4 projection;
  glm::mat4 view;

  float fovy, m_aspect;

  void updateMatrices() {
    view = glm::lookAt(location, lookat, upvec);
    projection = glm::perspective(
          glm::radians(fovy), m_aspect, ZNEAR, ZFAR);
  }

public:
  Camera(int viewportWidth, int viewportHeight) {
    location = glm::vec3(1, 1, 3);
    lookat   = glm::vec3(0, 0, 0);
    upvec    = glm::vec3(0, 1, 0);
    fovy  = 45.0f;
    m_aspect = (float) viewportWidth / (float) viewportHeight;

    updateMatrices();
  }

  void setProjection(glm::mat4& mat) { projection = mat; }
  void setView(glm::mat4& mat) { view = mat; }

  glm::mat4 matrix() const { return projection * view; }

  glm::vec3 getLocation() { return location; }

  float aspect() const { return m_aspect; }
  void  aspect(float ratio) { m_aspect = ratio; updateMatrices(); }

  void move(glm::vec3 translate) {
    location += translate;
    updateMatrices();
  }
};

}

#endif // CAMERA_H
