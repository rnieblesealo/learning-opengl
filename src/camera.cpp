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
    , _world_up(start_up)
    , _yaw(start_yaw)
    , _pitch(start_pitch)
    , _front(glm::vec3(0.0f, 0.0f, 1.0f)) // Facing into screen initially
    , _movement_speed(start_movement_speed)
    , _look_speed(start_look_speed)
{
  this->_Update();
}

Camera::~Camera() {}

void Camera::KeyControl(Window &window, float delta_time)
{
  auto keys = window.GetKeys();

  if (keys[GLFW_KEY_W])
  {
    this->_position += this->_front * this->_movement_speed * delta_time;
  }
  if (keys[GLFW_KEY_A])
  {
    this->_position -= this->_right * this->_movement_speed * delta_time;
  }
  if (keys[GLFW_KEY_S])
  {
    this->_position -= this->_front * this->_movement_speed * delta_time;
  }
  if (keys[GLFW_KEY_D])
  {
    this->_position += this->_right * this->_movement_speed * delta_time;
  }
}

void Camera::MouseControl(Window &window)
{
  float dx = window.GetDx() * this->_look_speed;
  float dy = window.GetDy() * this->_look_speed;

  this->_yaw += dx;
  this->_pitch += dy;

  this->_pitch = glm::clamp(this->_pitch, -90.0f, 90.0f); // You can't turn your head beyond your spine...

  this->_Update();

  // Clear deltas after cursor pos callback
  window.SetDx(0);
  window.SetDy(0);
}

glm::mat4 Camera::CalculateViewMatrix()
{
  return glm::lookAt(this->_position, this->_position + this->_front, this->_up);
}

void Camera::_Update()
{
  this->_front.x = glm::cos(glm::radians(this->_yaw)) * glm::cos(glm::radians(this->_pitch));
  this->_front.y = glm::sin(glm::radians(this->_pitch));
  this->_front.z = glm::sin(glm::radians(this->_yaw)) * glm::cos(glm::radians(this->_pitch));
  this->_front   = glm::normalize(this->_front);

  this->_right = glm::normalize(glm::cross(this->_front, this->_world_up));

  this->_up = glm::normalize(glm::cross(this->_right, this->_front));
}
} // namespace gle
