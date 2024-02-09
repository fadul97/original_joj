#include "renderer/opengl/texture_gl.h"

#include "vendor/stb/stb_image.h"
#include <iostream>

joj::GLTexture::GLTexture()
    : width{0}, height{0}, internal_format{GL_RGB}, image_format{GL_RGB},
      wrap_s{GL_REPEAT}, wrap_t{GL_REPEAT}, filter_min{GL_LINEAR}, filter_max{GL_NEAREST}
{
    glGenTextures(1, &id);
}

joj::GLTexture::GLTexture(const char* image_path, i32 internal_format, GLenum image_format)
    : width{0}, height{0}, internal_format{internal_format}, image_format{image_format},
      wrap_s{GL_REPEAT}, wrap_t{GL_REPEAT}, filter_min{GL_LINEAR}, filter_max{GL_LINEAR}
{
    glGenTextures(1, &id);

    i32 nr_channels;
    u8* data = stbi_load("textures/container.jpg", &width, &height, &nr_channels, 0);

    if (data)
    {
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 
            0, image_format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load image.\n";
    }
    
}

joj::GLTexture::~GLTexture()
{
}

void joj::GLTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void joj::GLTexture::create(i32 width, i32 height, u8* data, i32 internal_format, GLenum image_format)
{
    this->width = width;
    this->height = height;
    this->internal_format = internal_format;
    this->image_format = image_format;
    
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

    glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->width, this->height, 
        0, this->image_format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
}
