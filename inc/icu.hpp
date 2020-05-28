// input control unit
#ifndef ICU_HPP
#define ICU_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void glfw_key_callback(GLFWwindow* win, int key, int scode, int action, int mods);
void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods);
void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif//ICU_HPP
