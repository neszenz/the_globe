#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "profiler.hpp"
#include "util.hpp"
#include "window.h"

Window* g_window = new Window("the_globe", 960, 540, true, 4);
Window* g_window2 = new Window("the_globe", 960, 540, true, 4);
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
    assert(g_window2->IsValid());

    print_versions();

    while (g_window || g_window2) {
        PROFILER_RESET;

        PROFILE(compute_delta());

        if (g_window) {
            PROFILE(g_window->MakeContextCurrent());
            PROFILE(glClear(GL_COLOR_BUFFER_BIT));

            if (!g_window->ShouldClose()) {
                PROFILE(g_window->Update());
            } else {
                delete(g_window);
                g_window = nullptr;
            }
        }
        if (g_window2) {
            PROFILE(g_window2->MakeContextCurrent());
            PROFILE(glClear(GL_COLOR_BUFFER_BIT));

            if (!g_window2->ShouldClose()) {
                PROFILE(g_window2->Update());
            } else {
                delete(g_window2);
                g_window2 = nullptr;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm
        /* std::cout << string_from_seconds(delta) << ' ' << round(1/delta)  << " fps" << std::endl; */

        /* PROFILER_PRINT; */
    }

    return 0;
}
