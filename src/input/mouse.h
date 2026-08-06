#pragma once

#include "../core/defines.h"

#include <GLFW/glfw3.h>

class mouse
{
public:
    static void init(GLFWwindow* window);

    static bool is_button_pressed(int button);
    static void get_position(f64& x, f64& y);
    static f64 get_x();
    static f64 get_y();

private:
    static inline GLFWwindow* s_window = nullptr;
};
