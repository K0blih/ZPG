#include "Texture.h"
#include "Image.h"

#include <array>
#include <stdexcept>
#include <string>

Texture::Texture(const char* filePath, Mipmaps mipmaps, bool flipVertically) : target(GL_TEXTURE_2D)
{
    const Image image(filePath, flipVertically);

    GLint maxSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
    if (image.width > maxSize || image.height > maxSize)
    {
        throw std::runtime_error("2D texture exceeds GL_MAX_TEXTURE_SIZE");
    }

    glGenTextures(1, &id);
    if (id == 0)
    {
        throw std::runtime_error("Failed to create 2D texture");
    }

    GLint previousBinding = 0;
    GLint previousAlignment = 0;

    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousBinding);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);

    glBindTexture(target, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(target, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);

    if (mipmaps == Mipmaps::Enabled)
    {
        glGenerateMipmap(target);
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mipmaps == Mipmaps::Enabled ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
    glBindTexture(target, static_cast<GLuint>(previousBinding));
}

Texture::Texture(const std::array<const char*, 6>& faces) : target(GL_TEXTURE_CUBE_MAP)
{
    const std::array<Image, 6> images = {
        Image(faces[0], false),
        Image(faces[1], false),
        Image(faces[2], false),
        Image(faces[3], false),
        Image(faces[4], false),
        Image(faces[5], false)
    };

    GLint maxSize = 0;
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxSize);

    for (std::size_t i = 0; i < images.size(); i++)
    {
        const Image& image = images[i];
        if (image.width != image.height)
        {
            throw std::runtime_error("Cubemap face must be square: " + std::string(faces[i]));
        }
        if (image.width > maxSize)
        {
            throw std::runtime_error("Cubemap face exceeds GL_MAX_CUBE_MAP_TEXTURE_SIZE");
        }

        if (i > 0 && image.width != images[0].width)
        {
            throw std::runtime_error("All cubemap faces must have the same dimensions");
        }
    }

    glGenTextures(1, &id);
    if (id == 0)
    {
        throw std::runtime_error("Failed to create cubemap");
    }

    GLint previousBinding = 0;
    GLint previousAlignment = 0;

    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &previousBinding);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);

    glBindTexture(target, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (std::size_t i = 0; i < images.size(); i++)
    {
        const Image& image = images[i];
        const GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i);
        glTexImage2D(faceTarget, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
    glBindTexture(target, static_cast<GLuint>(previousBinding));
}

void Texture::bind(GLuint unit) const
{
    GLint maxUnits = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);

    if (unit >= static_cast<GLuint>(maxUnits))
    {
        throw std::out_of_range("Texture unit index is out of range");
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, id);
}

Texture::~Texture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
}
