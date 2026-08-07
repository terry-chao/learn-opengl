#include "mouse.h"

void mouse::init(GLFWwindow* window)
{
    s_window = window;
}

bool mouse::is_button_pressed(int button)
{
    if (!s_window)
        return false;

    return glfwGetMouseButton(s_window, button) == GLFW_PRESS;
}

void mouse::get_position(f64& x, f64& y)
{
    if (!s_window)
    {
        x = 0.0;
        y = 0.0;
        return;
    }

    glfwGetCursorPos(s_window, &x, &y);
}

f64 mouse::get_x()
{
    f64 x = 0.0, y = 0.0;
    get_position(x, y);
    return x;
}

f64 mouse::get_y()
{
    f64 x = 0.0, y = 0.0;
    get_position(x, y);
    return y;
}
