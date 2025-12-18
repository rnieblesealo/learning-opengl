#include "shader.h"
#include "util.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gle
{

// clang-format off
std::array<std::string, 13> _GLE_BASE_SHADER_UNIFORMS{
  "model",
  "projection",
  "view",
  "sampler",
  "eye_pos",
  "light_properties.color",
  "light_properties.direction",
  "light_properties.ambient_intensity",
  "light_properties.diffuse_intensity",
  "material_properties.shininess"
  "material_properties.specular_intensity"
};
// clang-format on

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

  // Build uniforms
  this->_BuildUniformTable();
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

GLuint Shader::GetUniformLocation(std::string uniform_name)
{
  auto loc = this->_uniform_table.find(uniform_name);
  if (loc == this->_uniform_table.end())
  {
    std::cerr << "Failed to find uniform '" << uniform_name << "'" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return loc->second;
}

void Shader::_BuildUniformTable()
{
  for (auto const &uniform : _GLE_BASE_SHADER_UNIFORMS)
  {
    GLint u_loc = glGetUniformLocation(this->_shader, uniform.c_str());
    if (u_loc == -1)
    {
      std::cerr << "Failed to get location for uniform Float '" << uniform << "'" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    this->_uniform_table.emplace(uniform, u_loc); // .emplace() adds new entry; I picked it cuz I like the syntax
  }
}

void Shader::UseShader() { glUseProgram(this->_shader); }
} // namespace gle
