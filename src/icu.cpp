#include "icu.hpp"

#include <iostream>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "engine.hpp"
#include "globe.hpp"
#include "util.hpp"
#include "window.h"

extern Window g_window;
extern Camera g_camera;
extern Globe g_globe;

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
            engine.momentum = glm::vec3(0.0f);
        } else if (action == GLFW_RELEASE) {
            engine.mouse.is_down = false;
        }
    }
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    double xmove = xpos - engine.mouse.xlast;
    double ymove = ypos - engine.mouse.ylast;

    if (engine.mouse.is_down) {
        // precision movement
        glm::vec2 v0_2D = glm::vec2(engine.mouse.xlast, engine.mouse.ylast);
        glm::vec2 v1_2D = glm::vec2(xpos, ypos);
        glm::vec3 v0 = arc_ball_mapping(v0_2D, g_window.GetSize());
        glm::vec3 v1 = arc_ball_mapping(v1_2D, g_window.GetSize());

        float angle = acos(std::max(-1.0f, std::min(1.0f, glm::dot(v0, v1))));
        glm::vec3 axis = glm::cross(v1, v0);
        g_camera.orbit(angle, axis, glm::vec3(0.0f, 0.0f, 0.0f));

        // momentum movement
        if (std::abs(xmove + ymove) < 3) {
            engine.momentum = glm::vec3(0.0f); // clear if barely moved/ stopped
        } else {
            float speed = 4;
            if (v1.z > 0.0) {
                engine.momentum.x += speed * -(xmove / g_window.GetSize().x);
                engine.momentum.y += speed * -(ymove / g_window.GetSize().y);
            } else {
                engine.momentum.z += speed * ((axis.z > 0.0f) ? angle : -angle);
            }
        }
    }

    engine.mouse.xlast = xpos;
    engine.mouse.ylast = ypos;
}
