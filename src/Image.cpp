#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>
#include <string>

Image::Image(const char* filePath, bool flipVertically)
{
    if (!filePath || filePath[0] == '\0')
    {
        throw std::invalid_argument("Image path is empty");
    }

    int originalChannels = 0;

    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    pixels = stbi_load(filePath, &width, &height, &originalChannels, STBI_rgb_alpha);
    if (!pixels)
    {
        throw std::runtime_error(std::string("Failed to load image: ") + filePath + "\n");
    }
}

Image::~Image()
{
    stbi_image_free(pixels);
}