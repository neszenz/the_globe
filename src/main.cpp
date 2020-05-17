#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "globe.hpp"
#include "gl_utils.h"
#include "profiler.hpp"
#include "shader.h"
#include "util.hpp"
#include "window.h"

Window g_window("the_globe", 960, 540, true, 0);
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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    assert(glGetError() == GL_NO_ERROR);
}

void render(const Window& window, Shader& shader, const Globe& globe) {
    reset_opengl_settings();

    float w_half = 0.5 * window.GetSize().x;
    float h_half = 0.5 * window.GetSize().y;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), w_half/h_half, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 matrix = projection * view * model;

    window.MakeContextCurrent();
    shader.Bind();
    shader.UniformMat4("u_matrix", matrix);

    globe.draw();
}

int main() {
    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());

        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        PROFILE(render(g_window, g_shader, g_globe));

        PROFILE(g_window.Update());
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //TODO rm

        //PROFILER_PRINT;
        //std::cout << string_from_seconds(g_delta) << ' ' << round(1/g_delta)  << " fps" << std::endl;
    }

    return 0;
}
