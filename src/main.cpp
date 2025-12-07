#include "util.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gle
{
const GLint     WINDOW_WIDTH  = 800;
const GLint     WINDOW_HEIGHT = 600;
const glm::vec3 FORWARD{0.0f, 0.0f, 1.0f};
const glm::vec3 UP{0.0f, 1.0f, 0.0f};

const std::filesystem::path VERTEX_SHADER_PATH("../src/glsl/vertex.glsl");
const std::filesystem::path FRAGMENT_SHADER_PATH("../src/glsl/fragment.glsl");

GLuint VAO, VBO, EBO;
GLuint shader;
GLuint uniform_model;

typedef enum Direction
{
  LEFT = 0,
  RIGHT
} Direction;

Direction tri_trans_dir       = RIGHT; // Direction where tri is moving right now
float     tri_trans           = 0.0f;  // Current triangle displacement
float     tri_trans_max       = 0.0f;  // Flip direction once we get this far
float     tri_trans_increment = 0.0f;  // Amount to move tri by every frame

float tri_rot           = 0.0f;  // Current tri rotation
float tri_rot_increment = 0.25f; // Rotate by

Direction tri_scale_dir       = RIGHT;
float     tri_scale           = 0.5f;
float     tri_scale_min       = 0.5f;
float     tri_scale_max       = 0.5f;
float     tri_scale_increment = 0.0f;

void CreateTriangle()
{
  // clang-format off
  std::array<GLfloat, 12> vertices = {
    -1.0f, -1.0f,  0.0f, 
     0.0f, -1.0f,  1.0f, // Back coord; #WARNING: Looks like Z goes into the screen!
     1.0f, -1.0f,  0.0f, 
     0.0f,  1.0f,  0.0f, 
  };

  // These form a shitty pyramid :)
  std::array<GLuint, 12> indices = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };
  // clang-format on

  glGenVertexArrays(1,
                    &VAO); // Create 1 VAO name in GPU memory; store ID at VAO. Pass GLuint array pointer for >1 VAOs
  glBindVertexArray(VAO);  // Render with this VAO

  glGenBuffers(1, &VBO);              // Create 1 VBO name in GPU memory; store ID at VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind our VBO name to the GL_ARRAY_BUFFER target
  glBufferData(GL_ARRAY_BUFFER,
               sizeof vertices, // WARNING: This needs to be BYTE SIZE, not element size
               vertices.data(),
               GL_STATIC_DRAW); // Allocate and initialize the VBO bound to GL_ARRAY_BUFFER with our vertex data

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices.data(), GL_STATIC_DRAW);

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

  glBindBuffer(GL_ARRAY_BUFFER, 0);         // Unbind VBO
  glBindVertexArray(0);                     // Unbind VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO, WARNING: Must be unbound AFTER VAO!
}

void AddShader(GLuint program, std::string const &shader_code, GLenum shader_type)
{
  // Create shader object

  GLuint the_shader = glCreateShader(shader_type); // Make empty shader of given type and return its ID

  // Assemble the source

  /* NOTE: We can stitch together multiple sources into one file
   * This is why we pack shader source into arrays
   */

  std::array<GLchar const *, 1> the_code{0};
  the_code[0] = shader_code.data();

  std::array<GLint, 1> code_length{0};
  code_length[0] = shader_code.size();

  glShaderSource(the_shader, 1, the_code.data(), code_length.data()); // Param 1 here means only one piece of source

  // Compile the shader

  glCompileShader(the_shader);

  GLint       result = 0;
  std::string error_log(1024, 0);

  glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(the_shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error compiling shader program: " << error_log << std::endl;

    return;
  }

  // Add compiled shader to program

  glAttachShader(program, the_shader);
}

void CompileShaders()
{
  shader = glCreateProgram();
  if (!shader)
  {
    std::cerr << "Error creating shader program" << std::endl;
    return;
  }

  std::string v_shader_code(gle::ReadWholeFile(VERTEX_SHADER_PATH));
  std::string f_shader_code(gle::ReadWholeFile(FRAGMENT_SHADER_PATH));

  // Compile & attach shader programs

  AddShader(shader, v_shader_code, GL_VERTEX_SHADER);
  AddShader(shader, f_shader_code, GL_FRAGMENT_SHADER);

  GLint       result = 0;
  std::string error_log(1024, 0);

  // Link: The "compilation" process

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error linking shader program: " << error_log << std::endl;

    return;
  }

  // Validate: Does this program work within our OpenGL setup?

  glBindVertexArray(gle::VAO); // WARNING: Must bind VAO to validate against it

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, error_log.size(), NULL, error_log.data());

    std::cerr << "Error validating shader program: " << error_log << std::endl;

    return;
  }

  glBindVertexArray(0);

  // Get uniforms

  uniform_model = glGetUniformLocation(shader, "model");
}
} // namespace gle

int main()
{
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
  gsl::not_null<GLFWwindow *> main_window =
      glfwCreateWindow(gle::WINDOW_WIDTH, gle::WINDOW_HEIGHT, "GLEngine", NULL, NULL);

  // Get window framebuffer size
  glm::ivec2 fb_size{};
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
    std::cerr << "Error initializing GLEW" << std::endl;

    glfwDestroyWindow(main_window);
    glfwTerminate();

    std::exit(EXIT_FAILURE);
  }

  // Extra features
  glEnable(GL_DEPTH_TEST);

  // Set viewport (draw area) to be full framebuffer
  glViewport(0, 0, fb_size.x, fb_size.y);

  gle::CreateTriangle();
  gle::CompileShaders();

  // Main loop
  while (!glfwWindowShouldClose(main_window))
  {
    glfwPollEvents();

    // UPDATE ---------------------------------------------------------------------------------------------------------

    // Move tri in the direction it's meant to be going in
    if (gle::tri_trans_dir == gle::Direction::RIGHT)
    {
      gle::tri_trans += gle::tri_trans_increment;
    }
    else if (gle::tri_trans_dir == gle::Direction::LEFT)
    {
      gle::tri_trans -= gle::tri_trans_increment;
    }

    // Switch tri direction when hitting max offset
    if (std::abs(gle::tri_trans) >= gle::tri_trans_max)
    {
      if (gle::tri_trans_dir == gle::Direction::LEFT)
      {
        gle::tri_trans_dir = gle::Direction::RIGHT;
      }

      else
      {
        gle::tri_trans_dir = gle::Direction::LEFT;
      }
    }

    // Rotate tri
    if (gle::tri_rot >= 360)
    {
      gle::tri_rot = 0;
    }

    gle::tri_rot += gle::tri_rot_increment;

    // Scale tri
    if (gle::tri_scale >= gle::tri_scale_max)
    {
      gle::tri_scale_dir = gle::Direction::LEFT;
    }
    else if (gle::tri_scale <= gle::tri_scale_min)
    {
      gle::tri_scale_dir = gle::Direction::RIGHT;
    }

    if (gle::tri_scale_dir == gle::Direction::LEFT)
    {
      gle::tri_scale -= gle::tri_scale_increment;
    }
    else if (gle::tri_scale_dir == gle::Direction::RIGHT)
    {
      gle::tri_scale += gle::tri_scale_increment;
    }

    // RENDER ---------------------------------------------------------------------------------------------------------

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background clear color
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // Fill color buffer with clear color, fill depth buffer with cleared depth color

    // clang-format off
    glUseProgram(gle::shader);

      glm::mat4 model(1.0f);

      model = glm::translate(model, glm::vec3(gle::tri_trans, gle::tri_trans, 0.0f));
      model = glm::rotate(model, glm::radians(gle::tri_rot), gle::UP);
      model = glm::scale(model, glm::vec3(gle::tri_scale, gle::tri_scale, 1.0f));

      glUniformMatrix4fv(gle::uniform_model,
                         1,        // Passing only 1 matrix
                         GL_FALSE, // Do NOT transpose
                         glm::value_ptr(model));

      glBindVertexArray(gle::VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gle::EBO);
          glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    glUseProgram(0);
    // clang-format on 

    glfwSwapBuffers(main_window); // 2 buffer system
  }
}
