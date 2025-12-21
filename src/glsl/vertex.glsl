#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec3 in_normal;

out vec2 texcoord;
out vec3 normal;
out vec3 frag_pos; // Technically the vertex pos, but we will use as fragment position for Phong calculation

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
  texcoord = in_texcoord;

  mat3 normal_matrix = mat3(transpose(inverse(model))); // TODO: Understand the maths here
  normal = normal_matrix * in_normal;

  // Phong model fragment position is relative to world coordinates, this is why we multiply by model
  frag_pos = (model * vec4(pos, 1.0f)).xyz;

  gl_Position = projection * view * model * vec4(pos, 1.0f);
}
