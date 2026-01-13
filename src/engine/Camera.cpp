#include <engine/Camera.h>

Camera::Camera(glm::vec3 initialPositionVector) { Pos = initialPositionVector; };

void Camera::handleKeyboardInput(camera_movement direction, float deltaTime) {
  float velocity = Speed * deltaTime;
  if (direction == FORWARD)
    Pos += velocity * Front;
  if (direction == BACKWARD)
    Pos -= velocity * Front;
  if (direction == LEFT)
    Pos -= Right * velocity;
  if (direction == RIGHT)
    Pos += Right * velocity;
}

void Camera::handleMouseInput(float xoffset, float yoffset) {

  xoffset *= Sensivity;
  yoffset *= Sensivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (Pitch > 89.0f)
    Pitch = 89.0f;
  if (Pitch < -89.0f)
    Pitch = -89.0f;

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  // look around matrix
  glm::vec3 lookDirection;
  lookDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  lookDirection.y = sin(glm::radians(Pitch));
  lookDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

  Front = glm::normalize(lookDirection);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
