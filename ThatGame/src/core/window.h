#pragma once

#include "defines.h"

#include <string>
#include <functional>

struct GLFWwindow;

struct window_props
{
    std::string title = "Learn OpenGL";
    u32 width = 1280;
    u32 height = 720;
};

class window
{
public:
    using event_callback_fn = std::function<void()>;

    explicit window(const window_props& props = window_props{});
    ~window();

    window(const window&) = delete;
    window& operator=(const window&) = delete;

    void on_update();

    u32 get_width() const { return m_data.width; }
    u32 get_height() const { return m_data.height; }
    bool should_close() const;
    GLFWwindow* get_native_handle() const { return m_window; }

    void set_vsync(bool enabled);
    bool is_vsync() const { return m_data.vsync; }

private:
    void init(const window_props& props);
    void shutdown();

    GLFWwindow* m_window = nullptr;

    struct window_data
    {
        std::string title;
        u32 width = 0;
        u32 height = 0;
        bool vsync = true;
    };

    window_data m_data;
};
