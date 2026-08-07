#include "application.h"
#include "logging.h"
#include "asserts.h"

#include "../input/keyboard.h"
#include "../input/mouse.h"

#include <GLFW/glfw3.h>

application* application::s_instance = nullptr;

application::application()
{
    TG_ASSERT(!s_instance, "Application already exists");
    s_instance = this;

    logger::init();
}

application::~application()
{
    shutdown();
    m_window.reset();
    logger::shutdown();
    s_instance = nullptr;
    glfwTerminate();
}

void application::run()
{
    init();

    float last_time = static_cast<float>(glfwGetTime());

    while (m_running && !m_window->should_close())
    {
        const float time = static_cast<float>(glfwGetTime());
        const float delta_time = time - last_time;
        last_time = time;

        if (keyboard::is_key_pressed(GLFW_KEY_ESCAPE))
            m_running = false;

        update(delta_time);
        render();

        m_window->on_update();
    }
}

void application::init()
{
    m_window = std::make_shared<window>(window_props{ m_title, m_width, m_height });

    keyboard::init(m_window->get_native_handle());
    mouse::init(m_window->get_native_handle());

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    u32 indices[] = { 0, 1, 2 };

    m_vertex_array = std::make_shared<vertex_array>();
    m_vertex_array->bind();

    std::shared_ptr<vertex_buffer> vb = std::make_shared<vertex_buffer>(vertices, static_cast<u32>(sizeof(vertices)));
    std::shared_ptr<index_buffer> ib = std::make_shared<index_buffer>(indices, 3);

    m_vertex_array->set_index_buffer(ib);
    m_vertex_array->add_vertex_buffer(vb);

    vb->bind();
    m_vertex_array->set_vertex_layout_attrib(0, 3, sizeof(float) * 3, nullptr);
    m_vertex_array->unbind();

    m_shader = std::make_shared<shader>(
        "assets/shaders/default_vertex.glsl",
        "assets/shaders/default_fragment.glsl");
}

void application::update(float delta_time)
{
    (void)delta_time;
}

void application::render()
{
    render_commands::set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
    render_commands::clear();

    m_shader->bind();
    render_commands::draw_indexed(m_vertex_array);
}

void application::shutdown()
{
    m_shader.reset();
    m_vertex_array.reset();
}
