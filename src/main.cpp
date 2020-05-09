#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "profiler.hpp"
#include "util.hpp"
#include "window.h"

Window g_window("the_globe", 960, 540, true, 4);

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
    assert(g_window.IsValid());

    print_gl_version();

    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());


        PROFILE(glClear(GL_COLOR_BUFFER_BIT));

        PROFILE(g_window.Update());
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        PROFILER_PRINT;
    }

    return 0;
}
