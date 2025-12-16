#include "mesh.h"
#include "shader.h"

#include <cstdlib>
#include <vector>

namespace gle
{
Mesh::Mesh(std::vector<GLfloat> const &vertices, std::vector<GLuint> const &indices, Shader &shader)
    : _shader(shader)
{
  // Gen VAO
  glGenVertexArrays(1, &this->_vao);
  glBindVertexArray(this->_vao);

  // Validate shader against VAO
  if (!shader.Validate(this->_vao))
  {
    std::exit(EXIT_FAILURE);
  }

  // Gen VBO
  glGenBuffers(1, &this->_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

  // Position vertex attributes
  glVertexAttribPointer(0,                           // Shader input location where to store position vertex attr.
                        3,                           // Amount of components in position (x, y, z make 3)
                        GL_FLOAT,                    // Data type of each vertex
                        GL_FALSE,                    // Normalize the vertices?
                        sizeof vertices.front() * 8, // Stride; jump this amount of bytes to get to next vertex
                        0                            // Offset from first VBO value where data actually begins
  );

  /* "Take the data described by glVertexAttribPointer at this index,
   * and feed it to the shader’s layout(location = index) variable."
   */
  glEnableVertexAttribArray(0);

  // Texture coordinate vertex attributes
  glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, sizeof vertices.front() * 8, reinterpret_cast<void *>(sizeof vertices.front() * 3));

  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2, 3, GL_FLOAT, GL_FALSE, sizeof vertices.front() * 8, reinterpret_cast<void *>(sizeof vertices.front() * 5));

  glEnableVertexAttribArray(2);

  // AttribPointer Pattern: Define for 0, set for 0...

  // Gen EBO
  glGenBuffers(1, &this->_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

  this->_index_count = indices.size();

  // Unbind all
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // WARNING: EBO must be unbound after VAO!
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &this->_ebo);
  glDeleteBuffers(1, &this->_vbo);
  glDeleteVertexArrays(1, &this->_vao);
}

void Mesh::Draw()
{
  // clang-format off
  glUseProgram(this->_shader.GetShaderIndex());
    glBindVertexArray(this->_vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
        glDrawElements(GL_TRIANGLES, this->_index_count, GL_UNSIGNED_INT, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  glUseProgram(0);
  // clang-format on
}
} // namespace gle
