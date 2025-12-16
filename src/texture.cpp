#include "texture.h"
#include <cstdlib>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gle
{
Texture::Texture(const std::filesystem::path loc)
    : _loc(loc)
    , _texture_id(0)
    , _width(0)
    , _height(0)
    , _bit_depth(0)
{
  // Load the texture
  stbi_set_flip_vertically_on_load(true); // Make texture upright
  uint8_t *texture_data =
      stbi_load(loc.c_str(), &this->_width, &this->_height, &this->_bit_depth, 0); // Last param is channel count
  if (!texture_data)
  {
    std::cout << "Failed to find texture '" << loc.c_str() << "'" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // clang-format off
  glGenTextures(1, &this->_texture_id);
  glBindTexture(GL_TEXTURE_2D, this->_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat on s axis wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat on t axis wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Nearest neighbor on zoom out 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Nearest neighbor on zoom in

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_width, this->_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data); // Set up texture data

    glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  // clang-format on

  stbi_image_free(texture_data);
}

Texture::~Texture()
{
  // TODO: Implement texture destructor
}

void Texture::UseTexture()
{
  glActiveTexture(GL_TEXTURE0);                    // Select active texture unit
  glBindTexture(GL_TEXTURE_2D, this->_texture_id); // Bind the texture ID we created to that unit
}
} // namespace gle
