#pragma once

#include "window.h"
#include "defines.h"

#include "../rendering/vertex_array.h"
#include "../rendering/gl_buffer.h"
#include "../rendering/shader.h"
#include "../rendering/render_commands.h"

#include <memory>
#include <string>

class application
{
public:
    application();
    ~application();

    application(const application&) = delete;
    application& operator=(const application&) = delete;

    void run();

    window& get_window() { return *m_window; }
    static application& get() { return *s_instance; }

private:
    void init();
    void update(float delta_time);
    void render();
    void shutdown();

    std::shared_ptr<window> m_window;
    std::shared_ptr<vertex_array> m_vertex_array;
    std::shared_ptr<shader> m_shader;

    std::string m_title = "ThatGame";
    u32 m_width = 1280;
    u32 m_height = 720;
    bool m_running = true;

    static application* s_instance;
};
