#pragma once

#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gle
{
class Mesh
{
public:
  Mesh(std::vector<GLfloat> const &vertices, std::vector<GLuint> const &indices, Shader &shader);
  ~Mesh();

  void Draw();
  void Validate(GLuint vao);

private:
  GLuint  _vao, _vbo, _ebo;
  GLsizei _index_count;
  Shader  _shader;
};
} // namespace gle
