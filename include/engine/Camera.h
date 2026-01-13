#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

enum camera_movement { FORWARD, BACKWARD, LEFT, RIGHT };
class Camera {
public:
  const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  // default yaw/pitch angle for looking around with camera
  float Yaw = -90.0f;
  float Pitch = 0.0f;

  // camera
  glm::vec3 Pos;
  glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Up;
  glm::vec3 Right = glm::normalize(glm::cross(Front, Up));

  float Speed = 2.5f;
  float Sensivity = 0.5f;

  Camera(glm::vec3 initialPositionVector);

  glm::vec4 getMatrix();
  void handleKeyboardInput(camera_movement direction, float deltaTime);
  void handleMouseInput(float xoffset, float yoffset);

private:
  void updateCameraVectors();
};

#endif
