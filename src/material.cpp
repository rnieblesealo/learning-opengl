#include "material.h"

namespace gle
{
Material::Material(glm::float32 specular_intensity, glm::float32 shininess)
    : _specular_intensity(specular_intensity)
    , _shininess(shininess)
{
}

Material::~Material() {}

glm::float32 Material::GetSpecularIntensity() { return this->_specular_intensity; }
glm::float32 Material::GetShininess() { return this->_shininess; }

} // namespace gle
