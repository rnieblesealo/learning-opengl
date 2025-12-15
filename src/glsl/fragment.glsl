#version 410 core

struct DirectionalLight {
  vec3 color;
  float intensity;
};

in vec2 texcoord;

out vec4 color;

uniform sampler2D sampler;
uniform DirectionalLight directional_light;

void main() {
  vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.intensity;
  color = texture(sampler, texcoord) * ambient_color;
}
