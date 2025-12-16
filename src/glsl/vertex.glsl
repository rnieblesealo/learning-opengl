#version 410 core // Metal 4.1

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;

out vec2 texcoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
  texcoord = in_texcoord;

  mat3 normal_matrix = mat3(transpose(inverse(model))); // TODO: Understand the maths here
  normal = normal_matrix * in_normal;

  gl_Position = projection * view * model * vec4(pos, 1.0f);
}
