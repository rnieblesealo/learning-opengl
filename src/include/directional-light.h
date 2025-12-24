#pragma once

#include "light.h"

namespace gle
{
class DirectionalLight final : public Light
{
public:
  DirectionalLight(glm::float32 r,
                   glm::float32 g,
                   glm::float32 b,
                   glm::float32 ambient_intensity,
                   glm::float32 diffuse_intensity,
                   glm::vec3    direction);
  ~DirectionalLight();

  glm::vec3 const &GetDirection();

private:
  glm::vec3 _direction;
};
} // namespace gle
