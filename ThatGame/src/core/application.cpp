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

    m_window = std::make_unique<window>(window_props{ "Learn OpenGL", 1280, 720 });

    keyboard::init(m_window->get_native_handle());
    mouse::init(m_window->get_native_handle());
}

application::~application()
{
    on_shutdown();
    m_window.reset();
    logger::shutdown();
    s_instance = nullptr;
    glfwTerminate();
}

void application::run()
{
    on_init();

    float last_time = static_cast<float>(glfwGetTime());

    while (m_running && !m_window->should_close())
    {
        const float time = static_cast<float>(glfwGetTime());
        const float delta_time = time - last_time;
        last_time = time;

        if (keyboard::is_key_pressed(GLFW_KEY_ESCAPE))
            m_running = false;

        on_update(delta_time);
        on_render();

        m_window->on_update();
    }
}
