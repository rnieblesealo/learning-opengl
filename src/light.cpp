#include "light.h"

namespace gle
{
Light::Light(glm::float32 r,
             glm::float32 g,
             glm::float32 b,
             glm::vec3    direction,
             glm::float32 ambient_intensity,
             glm::float32 diffuse_intensity)
    : _color(glm::vec3(r, g, b))
    , _direction(direction)
    , _ambient_intensity(ambient_intensity)
    , _diffuse_intensity(diffuse_intensity)
{
}

Light::~Light() {}

void Light::WriteLightProperties(Material &material)
{
  glUniform3f(material.GetShader().GetUniformLocation("directional_light.color"),
              this->_color.r,
              this->_color.g,
              this->_color.b);

  glUniform3f(material.GetShader().GetUniformLocation("directional_light.direction"),
              this->_direction.x,
              this->_direction.y,
              this->_direction.z);

  glUniform1f(material.GetShader().GetUniformLocation("directional_light.ambient_intensity"), this->_ambient_intensity);
  glUniform1f(material.GetShader().GetUniformLocation("directional_light.diffuse_intensity"), this->_diffuse_intensity);
}
} // namespace gle
