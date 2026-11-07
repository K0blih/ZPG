#pragma once

#include <glad/gl.h>

#include <array>

class Texture {
public:
    enum class Mipmaps { Enabled, Disabled };

    // 2D texture.
    explicit Texture(const char* filePath, Mipmaps mipmaps = Mipmaps::Enabled, bool flipVertically = true);
    // Cubemap: +X, -X, +Y, -Y, +Z, -Z.
    explicit Texture(const std::array<const char*, 6>& faces);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind(GLuint unit) const;

private:
    GLuint id = 0;
    GLenum target = GL_TEXTURE_2D;
};
