#version 410 core // Metal 4.1

layout(location = 0) in vec3 pos;

uniform mat4 model;

void main() {
  gl_Position = model * vec4(
        pos.x * 0.5,
        pos.y * 0.5,
        pos.z,
        1.0
      );
}
