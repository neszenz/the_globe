#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <thread>

#include "camera.hpp"
#include "engine.hpp"
#include "globe.hpp"
#include "gl_utils.h"
#include "profiler.hpp"
#include "shader.h"
#include "util.hpp"
#include "window.h"

Window g_window("the_globe", 960, 540, true, 0);
Camera g_camera(45.0f, g_window.GetAspect(), glm::vec3(0.0f, 0.0f, 4.0f));
Shader g_shader("basic", "res/shaders/basic.vert", "res/shaders/basic.frag");
Globe g_globe(23);
struct engine_t engine;

void compute_delta() {
    static double last_time = 0.0;
    double curr_time = glfwGetTime();

    engine.delta = curr_time - last_time;

    last_time = curr_time;
}

void process_orbit_momentum() {
    float speed = 0.2f;
    float smoothing = engine.delta * (1.0f / speed);
    glm::vec3 curr_momentum = smoothing * engine.momentum;
    engine.momentum -= curr_momentum;

    if (!engine.mouse.is_down) {
        g_camera.orbit_x(curr_momentum.x);
        g_camera.orbit_y(curr_momentum.y);
        g_camera.orbit_z(curr_momentum.z);
    }
}

void reset_opengl_settings() {
    GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GL(glEnable(GL_DEPTH_TEST));

    GL(glEnable(GL_CULL_FACE));
    GL(glFrontFace(GL_CCW));
    GL(glCullFace(GL_BACK));

    GL(glClearColor(1.0, 1.0, 1.0, 1.0));
}

int main() {
    g_window.MakeContextCurrent();
    reset_opengl_settings();
    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());
        PROFILE(process_orbit_momentum());
        PROFILE(g_camera.set_aspect(g_window.GetAspect()));

        // render calls
        PROFILE(GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
        PROFILE(g_globe.draw(g_shader, g_camera));

        PROFILE(g_window.Update());
        /* std::this_thread::sleep_for(std::chrono::milliseconds(6)); //TODO rm */

        //PROFILER_PRINT;
        //std::cout << string_from_seconds(engine.delta) << ' ' << round(1/engine.delta)  << " fps" << std::endl;
    }

    return 0;
}
