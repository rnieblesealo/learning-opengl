#pragma once

#include <glm/glm.hpp>

namespace gle
{
class Material
{
public:
  Material(glm::float32 specular_intensity, glm::float32 shininess);
  ~Material();

  glm::float32 GetSpecularIntensity();
  glm::float32 GetShininess();

private:
  glm::float32 _specular_intensity;
  glm::float32 _shininess;
};
} // namespace gle
