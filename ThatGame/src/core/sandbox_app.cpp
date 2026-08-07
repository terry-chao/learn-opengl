#include "sandbox_app.h"
#include "defines.h"
#include "logging.h"

#include <cstddef>

void sandbox_app::on_init()
{
    LOG_INFO("Sandbox initialized");

    constexpr f32 vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    constexpr u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    m_vao = std::make_shared<vertex_array>();
    m_vao->bind();

    auto vbo = std::make_shared<vertex_buffer>(vertices, static_cast<u32>(sizeof(vertices)));
    auto ibo = std::make_shared<index_buffer>(indices, static_cast<u32>(sizeof(indices) / sizeof(indices[0])));

    m_vao->add_vertex_buffer(vbo);
    m_vao->set_index_buffer(ibo);

    vbo->bind();
    m_vao->set_vertex_layout_attrib(0, 3, 3 * sizeof(f32), reinterpret_cast<const void*>(0));
    m_vao->unbind();

    m_shader = std::make_shared<shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");
}

void sandbox_app::on_render()
{
    render_commands::set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    render_commands::clear();

    m_shader->bind();
    render_commands::draw_indexed(m_vao);
}

void sandbox_app::on_shutdown()
{
    LOG_INFO("Sandbox shutdown");
    m_shader.reset();
    m_vao.reset();
}
