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

glm::vec3 const &Light::GetColor() { return this->_color; }
glm::vec3 const &Light::GetDirection() { return this->_direction; }
glm::float32     Light::GetAmbientIntensity() { return this->_ambient_intensity; }
glm::float32     Light::GetDiffuseIntensity() { return this->_diffuse_intensity; }
} // namespace gle
