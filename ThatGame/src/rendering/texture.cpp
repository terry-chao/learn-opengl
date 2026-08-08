#include "texture.h"
#include "../core/logging.h"
#include <stb_image.h>

texture::texture(const std::string& filepath)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    u8* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_n_channels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        GLenum internal_format = GL_RGB8;
        if (m_n_channels == 4)
        {
            format = GL_RGBA;
            internal_format = GL_RGBA8;
        }
        else if (m_n_channels == 1)
        {
            format = GL_RED;
            internal_format = GL_R8;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: {0} ({1})", filepath, stbi_failure_reason());
        // Incomplete texture samples as black; upload a 1x1 magenta placeholder.
        const u8 magenta[] = { 255, 0, 255, 255 };
        m_width = 1;
        m_height = 1;
        m_n_channels = 4;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, magenta);
    }
}

texture::~texture()
{
	delete_id();
}

void texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void texture::bind_slot(u32 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::delete_id()
{
	if (m_id)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}

void texture::set_data(const void* data, u32 width, u32 height, GLenum format)
{}
