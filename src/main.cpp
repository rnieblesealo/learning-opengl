#include "util.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>

namespace
{
const GLint WIDTH         = 800;
const GLint WINDOW_HEIGHT = 600;

const std::filesystem::path VERTEX_SHADER_PATH("../src/glsl/vertex.glsl");
const std::filesystem::path FRAGMENT_SHADER_PATH("../src/glsl/fragment.glsl");

GLuint VAO, VBO, shader;

void CreateTriangle()
{
  // clang-format off
  std::array<GLfloat, 9> vertices[] = {
    -1.0f, -1.0f, 0.0f, 
     1.0f, -1.0f, 0.0f, 
     0.0f,  1.0f, 0.0f
  };
  // clang-format on

  glGenVertexArrays(1,
                    &VAO); // Create 1 VAO name in GPU memory; store ID at VAO. Pass GLuint array pointer for >1 VAOs
  glBindVertexArray(VAO);  // Render with this VAO

  glGenBuffers(1, &VBO);              // Create 1 VBO name in GPU memory; store ID at VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind our VBO name to the GL_ARRAY_BUFFER target
  glBufferData(GL_ARRAY_BUFFER,
               vertices->size(),
               vertices,
               GL_STATIC_DRAW); // Allocate and initialize the VBO bound to GL_ARRAY_BUFFER with our vertex data

  // Specify how we'll interpret vertex data for a specific index in the shader
  glVertexAttribPointer(0,        // Index of vertex data in shader; layout(location = 0) here...
                        3,        // Amount of components in vertex; 3 for x, y, z
                        GL_FLOAT, // Data type of each vertex
                        GL_FALSE, // Normalize the vertices?
                        0, // Space between one vertex and the other; used to account for padding; 0 means there's none
                        0  // Offset from first VBO value where data actually begins
  );

  // Enable the vertex attrib pointer defined for index 0; the above one
  glEnableVertexAttribArray(0);

  // Pattern: Define for 0, set for 0...

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
  glBindVertexArray(0);             // Unbind VAO
}

void CompileShaders(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path)
{
  std::string vertex_shader_code(gle::ReadWholeFile(vertex_shader_path));
  std::string fragment_shader_code(gle::ReadWholeFile(fragment_shader_path));
}
} // namespace

int main()
{
  // Start GLFW
  if (glfwInit() != GLFW_TRUE)
  {
    std::exit(EXIT_FAILURE);
  }

  // Setup GLFW window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);            // Use core profile; essentially disables backwards compat
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Enable forward compatibility

  // Initialize window
  gsl::not_null<GLFWwindow *> main_window = glfwCreateWindow(WIDTH, WINDOW_HEIGHT, "GLEngine", NULL, NULL);

  // Get window framebuffer size
  glm::ivec2 fb_size;
  glfwGetFramebufferSize(main_window, &fb_size.x, &fb_size.y);

  // Set GLEW context
  // Essentially tell it that this window is the one bound to our OpenGL context
  // "Hey GLEW, draw stuff to this window!"
  glfwMakeContextCurrent(main_window);

  // Allow modern extensions
  glewExperimental = GL_TRUE;

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
  {
    glfwDestroyWindow(main_window);
    glfwTerminate();

    std::exit(EXIT_FAILURE);
  }

  // Set viewport (draw area) to be full framebuffer
  glViewport(0, 0, fb_size.x, fb_size.y);

  // Main loop
  while (!glfwWindowShouldClose(main_window))
  {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background clear color
    glClear(GL_COLOR_BUFFER_BIT);         // Clear background

    glfwSwapBuffers(main_window); // 2 buffer system
  }
}
