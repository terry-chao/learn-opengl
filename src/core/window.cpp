#include "window.h"
#include "logging.h"
#include "asserts.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

window::window(const window_props& props)
{
    init(props);
}

window::~window()
{
    shutdown();
}

void window::init(const window_props& props)
{
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    LOG_INFO("Creating window '{0}' ({1}x{2})", m_data.title, m_data.width, m_data.height);

    static bool glfw_initialized = false;
    if (!glfw_initialized)
    {
        const int success = glfwInit();
        TG_ASSERT(success, "Could not initialize GLFW");
        glfw_initialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(
        static_cast<int>(m_data.width),
        static_cast<int>(m_data.height),
        m_data.title.c_str(),
        nullptr,
        nullptr);

    TG_ASSERT(m_window, "Failed to create GLFW window");

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &m_data);

    const int glad_ok = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    TG_ASSERT(glad_ok, "Failed to initialize GLAD");

    LOG_INFO("OpenGL Info:");
    LOG_INFO("  Vendor:   {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    LOG_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("  Version:  {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    set_vsync(true);
}

void window::shutdown()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void window::on_update()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

bool window::should_close() const
{
    return glfwWindowShouldClose(m_window) != 0;
}

void window::set_vsync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_data.vsync = enabled;
}
