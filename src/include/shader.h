#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace gle
{
class Shader
{
public:
  Shader(std::string const &vertex_shader_path, std::string const &fragment_shader_path);
  ~Shader();

  void WriteUniformMat4(std::string const &uniform_name, glm::mat4 const &new_value);
  void WriteUniformVec3(std::string const &uniform_name, glm::vec3 const &new_value);
  void WriteUniformFloat(std::string const &uniform_name, glm::float32 new_value);

  bool   Validate(GLuint vao);
  GLuint GetShaderIndex();

private:
  GLuint _shader;

  GLuint _CompileShader(std::string const &shader_code, GLenum shader_type);
  void   _LinkProgram();
};
} // namespace gle
