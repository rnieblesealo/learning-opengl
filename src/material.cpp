#include "material.h"

namespace gle
{
Material::Material(Shader &shader, Texture const &texture, glm::float32 specular_intensity, glm::float32 shininess)
    : _shader(shader)
    , _texture(texture)
    , _specular_intensity(specular_intensity)
    , _shininess(shininess)
{
}

Material::~Material() {}

glm::float32 Material::GetSpecularIntensity() { return this->_specular_intensity; }
glm::float32 Material::GetShininess() { return this->_shininess; }
Shader      &Material::GetShader() { return this->_shader; }

void Material::UseMaterial()
{
  this->_shader.UseShader();
  this->_texture.UseTexture();
}

GLuint Material::GetShaderUniformLocation(std::string uniform_name)
{
  return this->_shader.GetUniformLocation(uniform_name);
}
} // namespace gle
