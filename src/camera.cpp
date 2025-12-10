#include "camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace gle
{

Camera::Camera(glm::vec3 start_position,
               glm::vec3 start_up,
               GLfloat   start_yaw,
               GLfloat   start_pitch,
               GLfloat   start_movement_speed,
               GLfloat   start_look_speed)
    : _position(start_position)
    , _up(start_up)
    , _yaw(start_yaw)
    , _pitch(start_pitch)
    , _movement_speed(start_movement_speed)
    , _look_speed(start_look_speed)
    , _front(glm::vec3(0.0f, 0.0f, 1.0f)) // Facing +Z/into screen initially
    , _world_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
  this->_Update();
}

Camera::~Camera() {}

void Camera::KeyControl(bool const *keys)
{
  if (keys[GLFW_KEY_W])
  {
    this->_position += this->_front * this->_movement_speed;
  }
  if (keys[GLFW_KEY_A])
  {
    this->_position -= this->_right * this->_movement_speed;
  }
  if (keys[GLFW_KEY_S])
  {
    this->_position -= this->_front * this->_movement_speed;
  }
  if (keys[GLFW_KEY_D])
  {
    this->_position += this->_right * this->_movement_speed;
  }
}

glm::mat4 Camera::CalculateViewMatrix()
{
  return glm::lookAt(this->_position, this->_position + this->_front, this->_world_up);
}

void Camera::_Update()
{
  this->_front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
  this->_front.y = sin(glm::radians(this->_pitch));
  this->_front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
  this->_front   = glm::normalize(this->_front);

  this->_right = glm::normalize(glm::cross(this->_front, this->_world_up));

  this->_up = glm::normalize(glm::cross(this->_right, this->_front));
}
} // namespace gle
