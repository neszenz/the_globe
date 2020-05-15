#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "profiler.hpp"
#include "util.hpp"
#include "window.h"

Window* g_window = new Window("the_globe", 960, 540, true, 4);
double g_delta = 0.0;

void print_versions() {
    std::string glfw_version(glfwGetVersionString());
    const unsigned char *opengl_version = glGetString(GL_VERSION);
    const unsigned char *vendor = glGetString(GL_VENDOR);
    const unsigned char *renderer = glGetString(GL_RENDERER);
    const unsigned char *glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    int numOfVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numOfVertexAttributes);
    std::cout << "using GLFW " << glfw_version << "\n"
              << "using OpenGL " << opengl_version << " | "
              << vendor << " | "
              << renderer << " | "
              << "GLSL " << glsl << " | "
              << "maxVertAttribs: " << numOfVertexAttributes << std::endl;
}

void compute_delta() {
    double curr_time = glfwGetTime();
    static double last_time = 0.0;

    g_delta = curr_time - last_time;

    last_time = curr_time;
}

int main() {
    assert(g_window->IsValid());

    print_versions();

    while (!g_window->ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());

        PROFILE(glClear(GL_COLOR_BUFFER_BIT));

        PROFILE(g_window->Update());
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        PROFILER_PRINT;
        std::cout << string_from_seconds(g_delta) << ' ' << round(1/g_delta)  << " fps" << std::endl;
    }

    return 0;
}
