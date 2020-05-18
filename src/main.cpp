#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "globe.hpp"
#include "gl_utils.h"
#include "profiler.hpp"
#include "shader.h"
#include "util.hpp"
#include "window.h"

// global vars (w/ prefix g_) are only to be defined here and accesed elsewhere
// via the extern statement
Window g_window("the_globe", 960, 540, true, 0);
Camera g_camera(45.0f, g_window.GetAspect(), glm::vec3(0.0f, 0.0f, 5.0f));
Shader g_shader("basic", "res/basic.vert", "res/basic.frag");
Globe g_globe(8);
double g_delta = -1.0;

void compute_delta() {
    double curr_time = glfwGetTime();
    static double last_time = 0.0;

    g_delta = curr_time - last_time;

    last_time = curr_time;
}

void reset_opengl_settings() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    assert(glGetError() == GL_NO_ERROR);
}

void render(const Window& window, Shader& shader, const Camera& camera, const Globe& globe) {
    reset_opengl_settings();

    glm::mat4 proj = camera.get_proj_matrix();
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 model = globe.get_model_matrix();
    glm::mat4 matrix = proj * view * model;

    window.MakeContextCurrent();
    shader.Bind();
    shader.UniformMat4("u_matrix", matrix);

    globe.draw();
}

int main() {
    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());
        PROFILE(g_globe.process_momentum(g_delta));
        PROFILE(g_camera.set_aspect(g_window.GetAspect()));

        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        PROFILE(render(g_window, g_shader, g_camera, g_globe));

        PROFILE(g_window.Update());
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        //PROFILER_PRINT;
        //std::cout << string_from_seconds(g_delta) << ' ' << round(1/g_delta)  << " fps" << std::endl;
    }

    return 0;
}
