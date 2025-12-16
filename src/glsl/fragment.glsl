#version 410 core

struct DirectionalLight {
  vec3 color;
  vec3 direction;
  float ambient_intensity;
  float diffuse_intensity;
};

in vec2 texcoord;
in vec3 normal;

out vec4 color;

uniform sampler2D sampler;
uniform DirectionalLight directional_light;

void main() {
  vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.ambient_intensity;

  float diffuse_factor = clamp(dot(normalize(normal), normalize(directional_light.direction)), 0.0f, 1.0f);
  vec4 diffuse_color = vec4(directional_light.color, 1.0f) * directional_light.diffuse_intensity * diffuse_factor;

  color = texture(sampler, texcoord) * (ambient_color + diffuse_color);
}
