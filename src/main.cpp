#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gle
{
const std::filesystem::path VERTEX_SHADER_PATH("glsl/vertex.glsl");
const std::filesystem::path FRAGMENT_SHADER_PATH("glsl/fragment.glsl");

const std::filesystem::path METAL_TEXTURE_PATH("assets/metal.jpg");

float tri_rot       = 0.0f; // Current tri rotation
float tri_rot_delta = 0.0f; // Rotate by this angle every frame

void CalcAverageNormals(std::vector<GLfloat> &vertices,
                        uint32_t              v_size,
                        std::vector<GLuint>  &indices,
                        uint32_t              n_offset)
{
  for (int i = 0; i < indices.size(); i += 3)
  {
    // Compute indices to vertices comprising this face
    uint32_t in0 = indices[i] * v_size;
    uint32_t in1 = indices[i + 1] * v_size;
    uint32_t in2 = indices[i + 2] * v_size;

    // Compute 2 edges of the face
    glm::vec3 v1(
        vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);

    glm::vec3 v2(
        vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

    // Get the face normal from their cross product
    glm::vec3 norm(glm::cross(v1, v2));
    norm = glm::normalize(norm);

    // Add the normal to each of the face vertices
    in0 += n_offset;
    in1 += n_offset;
    in2 += n_offset;

    vertices[in0] += norm.x;
    vertices[in0 + 1] += norm.y;
    vertices[in0 + 2] += norm.z;

    vertices[in1] += norm.x;
    vertices[in1 + 1] += norm.y;
    vertices[in1 + 2] += norm.z;

    vertices[in2] += norm.x;
    vertices[in2 + 1] += norm.y;
    vertices[in2 + 2] += norm.z;
  }

  for (int i = 0; i < vertices.size() / v_size; ++i)
  {
    uint32_t offset = i * v_size * n_offset;

    glm::vec3 vec(vertices[n_offset], vertices[n_offset + 1], vertices[n_offset + 2]);
    vec = glm::normalize(vec);

    vertices[n_offset]     = vec.x;
    vertices[n_offset + 1] = vec.y;
    vertices[n_offset + 2] = vec.z;
  }
}
} // namespace gle

int main()
{
  gle::Window window;

  gle::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 2.0f, 0.1f);
  gle::Light  directional_light(1.0f, 1.0f, 1.0f, glm::vec3(2.0f, -1.0f, -2.0f), 0.1f, 0.5f);

  gle::Shader   s_phong(gle::Shader(gle::VERTEX_SHADER_PATH, gle::FRAGMENT_SHADER_PATH));
  gle::Texture  t_metal(gle::METAL_TEXTURE_PATH);
  gle::Material pyramid_material(s_phong, t_metal, 0.6f, 10.0f); // Material with Phong shading and metallic texture

  // clang-format off
  std::vector<GLfloat> pyramid_vertices = {
  //    x      y     z       u     v      nx    ny    nz
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
     0.0f, -1.0f, 1.0f,   0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
  };

  // These form a shitty pyramid :)
  std::vector<GLuint> pyramid_indices = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };
  // clang-format on

  gle::CalcAverageNormals(pyramid_vertices, 8, pyramid_indices, 5);

  gle::Mesh pyramid(pyramid_vertices, pyramid_indices, pyramid_material);

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

    s_phong.WriteUniformMat4("model", model);

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

    s_phong.WriteUniformMat4("projection", projection);

    // Compute and pass view matrix
    s_phong.WriteUniformMat4("view", camera.CalculateViewMatrix());

    // Lighting
    s_phong.WriteUniformVec3("directional_light.color", directional_light.GetColor());
    s_phong.WriteUniformVec3("directional_light.direction", directional_light.GetDirection());
    s_phong.WriteUniformFloat("directional_light.ambient_intensity", directional_light.GetAmbientIntensity());
    s_phong.WriteUniformFloat("directional_light.diffuse_intensity", directional_light.GetDiffuseIntensity());

    // Camera
    s_phong.WriteUniformVec3("eye_pos", camera.GetPosition());

    // Material
    s_phong.WriteUniformFloat("material.shininess", pyramid_material.GetShininess());
    s_phong.WriteUniformFloat("material.specular_intensity", pyramid_material.GetSpecularIntensity());

    // RENDER ---------------------------------------------------------------------------------------------------------

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // clang-format off
    pyramid_material.UseMaterial();
      directional_light.WriteLightProperties(pyramid_material.Getu); // TODO: I don't like that we have to pass the material... Frick. INTERIM SOLN: Pass in table of uniform instead of whole material!
      pyramid.Draw(pyramid_material);
    // clang-format on

    glfwSwapBuffers(window.GetHandle());
  }
} // namespace gle
