#pragma once

#include <GLFW/glfw3.h>

class keyboard
{
public:
    static void init(GLFWwindow* window);
    static bool is_key_pressed(int keycode);

private:
    static inline GLFWwindow* s_window = nullptr;
};
