#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gle
{
const std::filesystem::path VERTEX_SHADER_PATH("glsl/vertex.glsl");
const std::filesystem::path FRAGMENT_SHADER_PATH("glsl/fragment.glsl");

const std::filesystem::path BRICK_TEXTURE_PATH("assets/brick.png");
const std::filesystem::path DIRT_TEXTURE_PATH("assets/dirt.png");
const std::filesystem::path OBAMA_TEXTURE_PATH("assets/obama.png");

float tri_rot       = 0.0f; // Current tri rotation
float tri_rot_delta = 0.0f; // Rotate by this angle every frame
} // namespace gle

int main()
{
  gle::Window window;
  gle::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 2.0f, 0.1f);
  gle::Light  directional_light(1.0f, 1.0f, 1.0f, 1.0f);

  // clang-format off
  std::vector<GLfloat> pyramid_vertices = {
  //    x      y     z       u     v
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,   1.0f, 1.0f,
     0.0f, -1.0f, 1.0f,   0.5f, 0.0f,
  };

  // These form a shitty pyramid :)
  std::vector<GLuint> pyramid_indices = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };
  // clang-format on

  gle::Shader pyramid_shader(gle::Shader(gle::VERTEX_SHADER_PATH, gle::FRAGMENT_SHADER_PATH));
  gle::Mesh   pyramid(pyramid_vertices, pyramid_indices, pyramid_shader);

  gle::Texture t_brick(gle::BRICK_TEXTURE_PATH);

  auto then = std::chrono::high_resolution_clock::now(); // Initial time for deltatime computation

  while (!glfwWindowShouldClose(window.GetHandle()))
  {
    // UPDATE ---------------------------------------------------------------------------------------------------------

    glfwPollEvents();

    // Compute delta time in seconds
    // Std. chrono provides high-res clock for this
    auto  now        = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float, std::chrono::seconds::period>(now - then).count();
    then             = now;

    // Camera movement
    camera.KeyControl(window, delta_time);
    camera.MouseControl(window);

    // Rotate triangle
    gle::tri_rot += gle::tri_rot_delta;
    if (gle::tri_rot >= 360.0f)
    {
      gle::tri_rot = 0.0f;
    }

    // Assemble model matrix
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // NOTE: When doing persp, Z into screen is negative!
    model = glm::rotate(model, glm::radians(gle::tri_rot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));

    pyramid_shader.WriteUniformMat4("model", model);

    // Update framebuffer size to keep aspect corrected
    window.PollFramebufferSize();
    glViewport(0, 0, window.GetFramebufferWidth(), window.GetFramebufferHeight()); // Must update viewport to match!

    // Assemble projection matrix
    glm::float32 fov  = glm::radians(60.0f);
    glm::float32 near = 0.1f;
    glm::float32 far  = 100.0f;

    glm::mat4 projection = glm::perspective(fov,
                                            static_cast<GLfloat>(window.GetFramebufferWidth()) /
                                                static_cast<GLfloat>(window.GetFramebufferHeight()),
                                            near,
                                            far);

    pyramid_shader.WriteUniformMat4("projection", projection);

    // Compute and pass view matrix
    pyramid_shader.WriteUniformMat4("view", camera.CalculateViewMatrix());

    // Lighting
    pyramid_shader.WriteUniformVec3("directional_light.color", directional_light.GetColor());
    pyramid_shader.WriteUniformFloat("directional_light.intensity", directional_light.GetIntensity());

    // RENDER ---------------------------------------------------------------------------------------------------------

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    t_brick.UseTexture();
    pyramid.Draw();

    glfwSwapBuffers(window.GetHandle());
  }
}
