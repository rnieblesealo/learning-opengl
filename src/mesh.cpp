#include "mesh.h"

#include <iostream>
#include <vector>

namespace gle
{
Mesh::Mesh(std::vector<GLfloat> const &vertices, std::vector<GLuint> const &indices, GLuint shader)
{
  // Gen VAO
  glGenVertexArrays(1, &this->_vao);
  glBindVertexArray(this->_vao);

  // Validate shader against VAO
  GLint       shader_validate_result = 0;
  std::string shader_validate_error_log(1024, 0);

  glValidateProgram(shader);

  glGetProgramiv(shader, GL_VALIDATE_STATUS, &shader_validate_result);
  if (!shader_validate_result)
  {
    glGetProgramInfoLog(shader, shader_validate_error_log.size(), NULL, shader_validate_error_log.data());

    std::cerr << "Error validating shader program: " << shader_validate_error_log << std::endl;

    std::exit(EXIT_FAILURE);
  }

  this->_shader = shader;

  // Get shader uniforms
  this->_u_model      = glGetUniformLocation(this->_shader, "model");
  this->_u_projection = glGetUniformLocation(this->_shader, "projection");

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
  // TBD
}

void Mesh::Draw(glm::mat4 const &model, glm::mat4 const &projection)
{
  // clang-format off
  glUseProgram(this->_shader);
    glUniformMatrix4fv(this->_u_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(this->_u_projection, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(this->_vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
        glDrawElements(GL_TRIANGLES, this->_index_count, GL_UNSIGNED_INT, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  glUseProgram(0);
  // clang-format on
}
} // namespace gle
