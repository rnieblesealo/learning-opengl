#pragma once

#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace gle
{
class Material
{
public:
  Material(Shader &shader, Texture const &texture, glm::float32 specular_intensity, glm::float32 shininess);
  ~Material();

  glm::float32 GetSpecularIntensity();
  glm::float32 GetShininess();

  Shader &GetShader();
  GLuint  GetShaderUniformLocation(std::string uniform_name);

  void UseMaterial();

private:
  glm::float32 _specular_intensity;
  glm::float32 _shininess;

  Shader  _shader;
  Texture _texture;
};
} // namespace gle
