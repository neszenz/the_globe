#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <chrono>
#include <thread>

#include "profiler.hpp"
#include "util.hpp"

GLFWwindow* g_window = nullptr;
std::string g_window_title = "the_globe";

void update_viewport(GLFWwindow*& win, int w = -1, int h = -1) {
    if (w < 1 || h < 1)
        glfwGetFramebufferSize(win, &w, &h);

    glViewport(0, 0, w, h);
}

void error_callback(int error, const char* description) {
    std::cout << "error_callback() - " << description << std::endl;
}

void key_callback(GLFWwindow* win, int key, int scode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* win, int w, int h) {
    update_viewport(win, w, h);
}

GLFWwindow* init_glfw() {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "init_glfw() error - glfwInit() failed" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* win = glfwCreateWindow(640, 480, g_window_title.c_str(),
                                       NULL, NULL);
    if (win == nullptr) {
        std::cout << "init_glfw() error - window is null" << std::endl;
        return nullptr;
    }

    glfwMakeContextCurrent(win);
    if (glewInit() != GLEW_OK) {
        std::cout << "init_glfw() error - glewInit() failed" << std::endl;
        return nullptr;
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(win, key_callback);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    glfwSwapInterval(1); // acts on current context
    update_viewport(win);

    return win;
}

void print_gl_version() {
    const unsigned char *version = glGetString(GL_VERSION);
    const unsigned char *vendor = glGetString(GL_VENDOR);
    const unsigned char *renderer = glGetString(GL_RENDERER);
    const unsigned char *glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    int numOfVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numOfVertexAttributes);
    std::cout << "using OpenGL " << version << " | "
        << vendor << " | "
        << renderer << " | "
        << "GLSL " << glsl << " | "
        << "maxVertAttribs: " << numOfVertexAttributes << std::endl;
}

void compute_delta() {
    double curr_time = glfwGetTime();
    static double last_time = 0.0;

    double delta = curr_time - last_time;
    std::cout << string_from_seconds(delta) << ' '
              << round(1/delta)  << " fps" << std::endl;

    last_time = curr_time;
}

int main() {
    g_window = init_glfw();
    if (!g_window)
        goto exit;

    print_gl_version();

    while (!glfwWindowShouldClose(g_window)) {
        PROFILER_RESET;

        PROFILE(compute_delta());

        PROFILE(glfwPollEvents());

        PROFILE(glClear(GL_COLOR_BUFFER_BIT));

        PROFILE(glfwSwapBuffers(g_window));
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        PROFILER_PRINT;
    }

exit:
    glfwDestroyWindow(g_window);
    glfwTerminate();

    return 0;
}
