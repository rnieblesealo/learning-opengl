#include "mesh.h"
#include "shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gle
{
const GLint WINDOW_WIDTH  = 500;
const GLint WINDOW_HEIGHT = 500;

glm::ivec2 fb_size; // Bounds of framebuffer

const std::filesystem::path VERTEX_SHADER_PATH("../src/glsl/vertex.glsl");
const std::filesystem::path FRAGMENT_SHADER_PATH("../src/glsl/fragment.glsl");

float tri_rot       = 0.0f;  // Current tri rotation
float tri_rot_delta = 0.35f; // Rotate by this angle every frame
} // namespace gle

int main()
{
  // ==================================================================================================================
  // GL SETUP
  // ==================================================================================================================

  // Start GLFW
  if (glfwInit() != GLFW_TRUE)
  {
    std::cerr << "Error initializing GLFW" << std::endl;

    std::exit(EXIT_FAILURE);
  }

  // Setup GLFW window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);            // Use core profile; essentially disables backwards compat
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Enable forward compatibility

  // Initialize window
  GLFWwindow *main_window = glfwCreateWindow(gle::WINDOW_WIDTH, gle::WINDOW_HEIGHT, "GLEngine", NULL, NULL);

  // Get initial framebuffer size
  glfwGetFramebufferSize(main_window, &gle::fb_size.x, &gle::fb_size.y);

  // Set GLEW context
  // Essentially tell it that this window is the one bound to our OpenGL context
  // "Hey GLEW, draw stuff to this window!"
  glfwMakeContextCurrent(main_window);

  // Allow modern extensions
  glewExperimental = GL_TRUE;

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Error initializing GLEW" << std::endl;

    glfwDestroyWindow(main_window);
    glfwTerminate();

    std::exit(EXIT_FAILURE);
  }

  // Set viewport (draw area) to be full framebuffer
  glViewport(0, 0, gle::fb_size.x, gle::fb_size.y);

  // glEnable turns on additional OpenGL features
  // This enables depth testing
  glEnable(GL_DEPTH_TEST);

  // ==================================================================================================================
  // PYRAMID SETUP
  // ==================================================================================================================

  // clang-format off
  std::vector<GLfloat> pyramid_vertices = {
    -1.0f, -1.0f,  0.0f, 
     0.0f, -1.0f,  1.0f, // Back coord 
     1.0f, -1.0f,  0.0f, 
     0.0f,  1.0f,  0.0f, 
  };

  // These form a shitty pyramid :)
  std::vector<GLuint> pyramid_indices = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };
  // clang-format on

  GLuint pyramid_shader = gle::Shader::MakeShaderProgram(gle::VERTEX_SHADER_PATH, gle::FRAGMENT_SHADER_PATH);

  gle::Mesh pyramid(pyramid_vertices, pyramid_indices, pyramid_shader);

  // ==================================================================================================================
  // MAIN LOOP
  // ==================================================================================================================

  while (!glfwWindowShouldClose(main_window))
  {
    glfwPollEvents();

    // UPDATE ---------------------------------------------------------------------------------------------------------

    // Rotate triangle
    gle::tri_rot += gle::tri_rot_delta;
    if (gle::tri_rot >= 360.0f)
    {
      gle::tri_rot = 0.0f;
    }

    // Assemble model matrix
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // NOTE: When doing persp, Z into screen is negative!
    model = glm::rotate(model, glm::radians(gle::tri_rot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));

    // Update framebuffer size to keep aspect corrected
    glfwGetFramebufferSize(main_window, &gle::fb_size.x, &gle::fb_size.y);
    glViewport(0, 0, gle::fb_size.x, gle::fb_size.y); // Must update viewport to match!

    // Assemble projection matrix
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f), static_cast<GLfloat>(gle::fb_size.x) / static_cast<GLfloat>(gle::fb_size.y), 0.1f, 100.0f);

    // RENDER ---------------------------------------------------------------------------------------------------------

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background clear color
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // Fill color buffer with clear color, fill depth buffer with cleared depth color

    pyramid.Draw(model, projection);

    glfwSwapBuffers(main_window); // 2 buffer system
  }
}
