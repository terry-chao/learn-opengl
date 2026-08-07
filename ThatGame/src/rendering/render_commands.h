#pragma once

#include "vertex_array.h"

#include <glad/glad.h>
#include <memory>

namespace render_commands
{
    inline void set_clear_color(f32 r, f32 g, f32 b, f32 a)
    {
        glClearColor(r, g, b, a);
    }

    inline void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT)
    {
        glClear(mask);
    }

    inline void draw_indexed(const std::shared_ptr<vertex_array>& vao, u32 index_count = 0)
    {
        vao->bind();
        const u32 count = index_count ? index_count : vao->get_index_buffer()->get_count();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
    }

    inline void draw_arrays(const std::shared_ptr<vertex_array>& vao, u32 vertex_count, GLenum mode = GL_TRIANGLES)
    {
        vao->bind();
        glDrawArrays(mode, 0, static_cast<GLsizei>(vertex_count));
    }

    inline void set_viewport(u32 x, u32 y, u32 width, u32 height)
    {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }
}
