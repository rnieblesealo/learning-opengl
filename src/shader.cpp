#include "shader.h"
#include "util.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gle
{
Shader::Shader(std::string const &vertex_shader_path, std::string const &fragment_shader_path)
{
  this->_shader = glCreateProgram();
  if (!_shader)
  {
    std::cerr << "Error creating shader program" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::string vertex_shader_code(gle::ReadWholeFile(vertex_shader_path));
  std::string fragment_shader_code(gle::ReadWholeFile(fragment_shader_path));

  // Compile
  GLuint vertex_shader = this->_CompileShader(vertex_shader_code, GL_VERTEX_SHADER);
  if (vertex_shader > 0)
  {
    glAttachShader(this->_shader, vertex_shader);
  }

  GLuint fragment_shader = this->_CompileShader(fragment_shader_code, GL_FRAGMENT_SHADER);
  if (fragment_shader > 0)
  {
    glAttachShader(this->_shader, fragment_shader);
  }

  // Link
  this->_LinkProgram();
}

Shader::~Shader() { glDeleteProgram(this->_shader); }

GLuint Shader::_CompileShader(std::string const &shader_code, GLenum shader_type)
{
  GLuint shader = glCreateShader(shader_type);

  // NOTE: We can stitch together multiple sources into one file; this is why we pack shader source into arrays

  std::array<GLchar const *, 1> _shader_code{0};
  _shader_code[0] = shader_code.data();

  std::array<GLint, 1> shader_code_length{0};
  shader_code_length[0] = shader_code.size();

  glShaderSource(shader, 1, _shader_code.data(), shader_code_length.data());

  GLint       result = 0;
  std::string error_log(1024, 0);

  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error compiling shader program: " << error_log << std::endl;
    return 0; // NOTE: Valid GL shader cannot be 0; hence it's safe to return this here
  }

  return shader;
}

void Shader::_LinkProgram()
{
  GLint       result = 0;
  std::string error_log(1024, 0);

  glLinkProgram(this->_shader);

  glGetProgramiv(this->_shader, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(this->_shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error linking shader program: " << error_log << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

bool Shader::Validate(GLuint vao)
{
  GLint       validate_res = 0;
  std::string shader_validate_error_log(1024, 0);

  glValidateProgram(this->_shader);

  glGetProgramiv(this->_shader, GL_VALIDATE_STATUS, &validate_res);
  if (!validate_res)
  {
    glGetProgramInfoLog(this->_shader, shader_validate_error_log.size(), NULL, shader_validate_error_log.data());

    std::cerr << "Error validating shader program: " << shader_validate_error_log << std::endl;
    return false;
  }

  return true;
}

GLuint Shader::GetShaderIndex() { return this->_shader; }

void Shader::WriteUniformMat4(std::string const &uniform_name, glm::mat4 const &new_value)
{
  // WARNING: Shader must be bound!
  glUseProgram(this->_shader);

  GLint u_loc = glGetUniformLocation(this->_shader, uniform_name.c_str());
  if (u_loc == -1)
  {
    std::cerr << "Failed to get location for uniform Mat4 '" << uniform_name << "'" << std::endl;
  }

  glUniformMatrix4fv(u_loc, 1, GL_FALSE, glm::value_ptr(new_value));

  glUseProgram(0);
}

void Shader::WriteUniformVec3(std::string const &uniform_name, glm::vec3 const &new_value)
{
  glUseProgram(this->_shader);

  GLint u_loc = glGetUniformLocation(this->_shader, uniform_name.c_str());
  if (u_loc == -1)
  {
    std::cerr << "Failed to get location for uniform Vec3 '" << uniform_name << "'" << std::endl;
  }

  glUniform3f(u_loc, new_value.x, new_value.y, new_value.z);

  glUseProgram(0);
}

void Shader::WriteUniformFloat(std::string const &uniform_name, glm::float32 new_value)
{
  glUseProgram(this->_shader);

  GLint u_loc = glGetUniformLocation(this->_shader, uniform_name.c_str());
  if (u_loc == -1)
  {
    std::cerr << "Failed to get location for uniform Float '" << uniform_name << "'" << std::endl;
  }

  glUniform1f(u_loc, new_value);

  glUseProgram(0);
};
} // namespace gle
