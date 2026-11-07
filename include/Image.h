#pragma once

struct Image {
    explicit Image(const char* filePath, bool flipVertically = true);
    ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    int width = 0;
    int height = 0;
    unsigned char* pixels = nullptr;
};