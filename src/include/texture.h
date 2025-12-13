#pragma once

#include <GL/glew.h>
#include <filesystem>

namespace gle
{

// TODO: (slash NOTE): Architecturally, textures are bound to meshes; address the design of this!
class Texture
{
public:
  Texture(const std::filesystem::path loc);
  ~Texture();

  void UseTexture();

private:
  std::filesystem::path _loc;
  GLuint                _texture_id;

  // Kept as int since stbi_load expects them as such
  int _width;
  int _height;
  int _bit_depth;
};
}; // namespace gle
