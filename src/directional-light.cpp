#include "directional-light.h"

namespace gle
{
DirectionalLight::DirectionalLight(glm::float32 r,
                                   glm::float32 g,
                                   glm::float32 b,
                                   glm::float32 ambient_intensity,
                                   glm::float32 diffuse_intensity,
                                   glm::vec3    direction)
    : Light(r, g, b, ambient_intensity, diffuse_intensity)
    , _direction(direction)
{
}

DirectionalLight::~DirectionalLight() {}

glm::vec3 const &DirectionalLight::GetDirection() { return this->_direction; }

} // namespace gle
