#version 410 core // Metal 4.1

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 v_color;

void main() {
  v_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
  gl_Position = projection * view * model * vec4(pos, 1.0f);
}
