#pragma once

#include <GL/glew.h>
#include <string>

namespace gle
{
class Shader
{
public:
  static void   AddShader(GLuint program, std::string const &shader_code, GLenum shader_type);
  static GLuint MakeShaderProgram(std::string const &vertex_shader_path, std::string const &fragment_shader_path);
};
} // namespace gle
