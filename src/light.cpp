#include "light.h"

namespace gle
{
Light::Light(glm::float32 r, glm::float32 g, glm::float32 b, glm::float32 intensity)
    : _color(glm::vec3(r, g, b))
    , _ambient_intensity(intensity)
{
}

Light::~Light() {}

glm::vec3 const &Light::GetColor() { return this->_color; }
glm::float32     Light::GetIntensity() { return this->_ambient_intensity; }
} // namespace gle
