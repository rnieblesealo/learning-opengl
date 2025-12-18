#version 410 core

struct DirectionalLight {
  vec3 color;
  vec3 direction;
  float ambient_intensity;
  float diffuse_intensity;
};

struct Material {
  float shininess;
  float specular_intensity;
};

in vec2 texcoord;
in vec3 normal;
in vec3 frag_pos;

out vec4 color;

uniform sampler2D sampler;
uniform DirectionalLight directional_light;
uniform Material material;
uniform vec3 eye_pos;

void main() {
  // Normalize everything
  vec3 N = normalize(normal);
  vec3 L = normalize(directional_light.direction);

  vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.ambient_intensity;

  float diffuse_factor = clamp(dot(N, L), 0.0f, 1.0f);
  vec4 diffuse_color = vec4(directional_light.color, 1.0f) * directional_light.diffuse_intensity * diffuse_factor;

  vec4 specular_color = vec4(0, 0, 0, 0);

  // If there isn't even diffuse light there can't be specular light! Keep it at 0
  if (diffuse_factor > 0) {
    // If these 2 are perfectly parallel we will see bright white
    vec3 frag_to_eye = eye_pos - frag_pos;
    vec3 R = normalize(reflect(L, N));

    float specular_factor = dot(frag_to_eye, R);
    if (specular_factor > 0) {
      specular_factor = pow(specular_factor, material.shininess);
      specular_color = vec4(directional_light.color * material.specular_intensity * specular_factor, 1.0f);
    }

    color = texture(sampler, texcoord) * (ambient_color + diffuse_color + specular_color);
  }
}
