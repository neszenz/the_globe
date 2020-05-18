#include "icu.hpp"

#include <glm/glm.hpp>

#include "camera.hpp"
#include "engine.hpp"
#include "globe.hpp"

extern Globe g_globe;
extern Camera g_camera;

void glfw_key_callback(GLFWwindow* win, int key, int scode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q)
            glfwSetWindowShouldClose(win, GLFW_TRUE);

        if (key == GLFW_KEY_C) {
            g_camera.reset_position();
            g_camera.reset_orientation();
        }

        if (key == GLFW_KEY_J)
            g_globe.add_momentum(1.0f, 0.0f, 0.0f);
        if (key == GLFW_KEY_K)
            g_globe.add_momentum(0.0f, 1.0f, 0.0f);
        if (key == GLFW_KEY_L)
            g_globe.add_momentum(0.0f, 0.0f, 1.0f);
    }
}

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            engine.mouse.is_down = true;
            engine.momentum = glm::vec2(0.0f, 0.0f);
            /* glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); */
        } else if (action == GLFW_RELEASE) {
            engine.mouse.is_down = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    double xmove = xpos - engine.mouse.xlast;
    double ymove = ypos - engine.mouse.ylast;

    if (engine.mouse.is_down) {
        double smoothing = 200.0;
        engine.momentum.x -= xmove / smoothing;
        engine.momentum.y -= ymove / smoothing;
    }

    engine.mouse.xlast = xpos;
    engine.mouse.ylast = ypos;
}
