#include <engine/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 initialPositionVector) {
  Position = initialPositionVector;
  updateCameraVectors();
};

void Camera::handleKeyboardInput(camera_movement direction, float deltaTime) {
  float velocity = Speed * deltaTime;
  if (direction == FORWARD)
    Position += velocity * Front;
  if (direction == BACKWARD)
    Position -= velocity * Front;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
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

glm::mat4 Camera::getViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

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
