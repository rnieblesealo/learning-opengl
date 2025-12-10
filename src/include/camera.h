#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace gle
{
class Camera
{
public:
  Camera(glm::vec3 start_position,
         glm::vec3 start_up,
         GLfloat   start_yaw,
         GLfloat   start_pitch,
         GLfloat   start_movement_speed,
         GLfloat   start_look_speed);
  ~Camera();

  void      KeyControl(bool const *keys);
  glm::mat4 CalculateViewMatrix();

private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
  glm::vec3 _right;
  glm::vec3 _world_up;

  GLfloat _yaw;
  GLfloat _pitch;
  GLfloat _movement_speed;
  GLfloat _look_speed;

  void _Update();
};
} // namespace gle
