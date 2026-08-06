#pragma once

#include "gl_base_element.h"

#include <glad/glad.h>

class vertex_buffer : public gl_base_element
{
public:
    vertex_buffer(const void* data, u32 size);
    ~vertex_buffer() override;

    void bind() override;
    void unbind() override;
    void delete_id() override;

    void set_data(const void* data, u32 size);
};

class index_buffer : public gl_base_element
{
public:
    index_buffer(const u32* data, u32 count);
    ~index_buffer() override;

    void bind() override;
    void unbind() override;
    void delete_id() override;

    u32 get_count() const { return m_count; }

private:
    u32 m_count = 0;
};
