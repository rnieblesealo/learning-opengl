#pragma once

#include "material.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace gle
{
class Light
{
public:
  Light(glm::float32 r,
        glm::float32 g,
        glm::float32 b,
        glm::vec3    direction,
        glm::float32 ambient_intensity,
        glm::float32 diffuse_intensity);
  ~Light();

  void WriteLightProperties(Material &material);

private:
  glm::vec3    _direction;
  glm::vec3    _color;
  glm::float32 _ambient_intensity;
  glm::float32 _diffuse_intensity;
};
} // namespace gle
