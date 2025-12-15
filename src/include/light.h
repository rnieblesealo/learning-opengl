#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace gle
{
class Light
{
public:
  Light(glm::float32 r, glm::float32 g, glm::float32 b, glm::float32 intensity);
  ~Light();

  glm::vec3 const &GetColor();
  glm::float32     GetIntensity();

private:
  glm::vec3    _color;
  glm::float32 _ambient_intensity; // TODO: Should this be glm float?
};
} // namespace gle
