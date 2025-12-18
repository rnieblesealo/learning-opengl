#pragma once

#include "material.h"
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
  Mesh(std::vector<GLfloat> const &vertices, std::vector<GLuint> const &indices, Material &material);
  ~Mesh();

  void Draw(Material &material);

private:
  GLuint  _vao, _vbo, _ebo;
  GLsizei _index_count;

  glm::vec3    _translation;
  glm::float32 _rotation; // WARNING: Only rotates around Y for now
  glm::vec3    _scale;
};
} // namespace gle
