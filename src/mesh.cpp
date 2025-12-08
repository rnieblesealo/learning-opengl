#include "mesh.h"

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
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_STATIC_DRAW);

  // Set up vertex attributes
  glVertexAttribPointer(0,        // Index of vertex data in shader; layout(location = 0) here...
                        3,        // Amount of components in vertex; 3 for x, y, z
                        GL_FLOAT, // Data type of each vertex
                        GL_FALSE, // Normalize the vertices?
                        0, // Space between one vertex and the other; used to account for padding; 0 means there's none
                        0  // Offset from first VBO value where data actually begins
  );

  glEnableVertexAttribArray(0);

  // AttribPointer Pattern: Define for 0, set for 0...

  // Gen EBO
  glGenBuffers(1, &this->_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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
