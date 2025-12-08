#pragma once

#include <GL/glew.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gle
{
class Mesh
{
public:
  Mesh(std::vector<GLfloat> const &vertices, std::vector<GLuint> const &indices, GLuint shader);
  ~Mesh();

  void Draw(glm::mat4 const &model, glm::mat4 const &projection);

private:
  GLuint _vao, _vbo, _ebo;
  GLuint _shader;
  GLuint _u_model, _u_projection;

  uint16_t _index_count;
};
} // namespace gle
