#include "keyboard.h"

void keyboard::init(GLFWwindow* window)
{
    s_window = window;
}

bool keyboard::is_key_pressed(int keycode)
{
    if (!s_window)
        return false;

    return glfwGetKey(s_window, keycode) == GLFW_PRESS;
}
