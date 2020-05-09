#include "icu.hpp"

void glfw_key_callback(GLFWwindow* win, int key, int scode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GLFW_TRUE);
}
