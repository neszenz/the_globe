#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "profiler.hpp"
#include "util.hpp"
#include "window.h"
#include "shader.h"
#include "gl_utils.h"

Window g_window("the_globe", 960, 540, true, 4);
Shader g_shader("basic", "res/basic.vert", "res/basic.frag");
double g_delta = -1.0;

void compute_delta() {
    double curr_time = glfwGetTime();
    static double last_time = 0.0;

    g_delta = curr_time - last_time;

    last_time = curr_time;
}

int main() {
    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());

        PROFILE(glClear(GL_COLOR_BUFFER_BIT));

        PROFILE(g_window.Update());
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        //PROFILER_PRINT;
        //std::cout << string_from_seconds(g_delta) << ' ' << round(1/g_delta)  << " fps" << std::endl;
    }

    return 0;
}
