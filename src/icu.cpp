#include "icu.hpp"

#include <glm/glm.hpp>

#include "camera.hpp"
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
