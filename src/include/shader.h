#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace gle
{
class Shader
/**
 * BASE UNIFORMS FOR GLE SHADER
 *
 * mat4               model
 * mat4               projection
 * mat4               view
 * sampler2D          sampler
 * vec3               eye_pos
 * LightProperties    light_properties
 *    vec3              .color
 *    vec3              .direction
 *    float             .ambient_intensity
 *    float             .diffuse_intensity
 * MaterialProperties material_properties
 *    float             .shininess
 *    float             .specular_intensity
 */
{
public:
  Shader(std::string const &vertex_shader_path, std::string const &fragment_shader_path);
  ~Shader();

  GLuint GetShaderIndex();
  GLuint GetUniformLocation(std::string uniform_name);

  bool Validate(GLuint vao);
  void UseShader();

private:
  GLuint                        _shader;
  std::map<std::string, GLuint> _uniform_table;

  GLuint _CompileShader(std::string const &shader_code, GLenum shader_type);
  void   _LinkProgram();
  void   _BuildUniformTable();
};
} // namespace gle
