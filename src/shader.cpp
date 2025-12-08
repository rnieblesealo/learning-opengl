#include "shader.h"
#include "util.h"
#include <iostream>

namespace gle
{
void Shader::AddShader(GLuint program, std::string const &shader_code, GLenum shader_type)
{
  // Create shader object index
  GLuint this_shader = glCreateShader(shader_type); // Make empty shader of given type and return its ID

  // Marshal source
  // NOTE: We can stitch together multiple sources into one file; this is why we pack shader source into arrays
  std::array<GLchar const *, 1> the_code{0};
  the_code[0] = shader_code.data();

  std::array<GLint, 1> code_length{0};
  code_length[0] = shader_code.size();

  glShaderSource(this_shader, 1, the_code.data(), code_length.data()); // Param 1 here means only one piece of source

  // Compile shader
  GLint       result = 0;
  std::string error_log(1024, 0);

  glCompileShader(this_shader);

  glGetShaderiv(this_shader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(this_shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error compiling shader program: " << error_log << std::endl;

    return;
  }

  // Add compiled shader to program
  glAttachShader(program, this_shader);
}

GLuint Shader::MakeShaderProgram(std::string const &vertex_shader_path, std::string const &fragment_shader_path)
{
  // Create program index
  GLuint shader = glCreateProgram();
  if (!shader)
  {
    std::cerr << "Error creating shader program" << std::endl;
    return 0; // NOTE: 0 is never a valid shader index in OpenGL; safe to return on error
  }

  std::string v_shader_code(gle::ReadWholeFile(vertex_shader_path));
  std::string f_shader_code(gle::ReadWholeFile(fragment_shader_path));

  // Compile
  AddShader(shader, v_shader_code, GL_VERTEX_SHADER);
  AddShader(shader, f_shader_code, GL_FRAGMENT_SHADER);

  // Link
  GLint       result = 0;
  std::string error_log(1024, 0);

  glLinkProgram(shader);

  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error linking shader program: " << error_log << std::endl;

    return 0;
  }

  // OK!
  return shader;
}
} // namespace gle
