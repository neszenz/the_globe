#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <assert.h>
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <sstream>
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
Globe g_globe_0(LOD_0), g_globe_1(LOD_1), g_globe_2(LOD_2);
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

void setup_ImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::getIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window.GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // style settings
    ImGuiStyle& style = ImGui::GetStyle();
    style.GrabRounding = 0.0f;
    style.WindowRounding = 0.0f;
}

void reset_opengl_settings() {
    GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GL(glEnable(GL_DEPTH_TEST));

    GL(glEnable(GL_CULL_FACE));
    GL(glFrontFace(GL_CCW));
    GL(glCullFace(GL_BACK));

    GL(glClearColor(1.0, 1.0, 1.0, 1.0));
}

void render_globe() {
    glm::mat4 proj = g_camera.get_proj_matrix();
    glm::mat4 view = g_camera.get_view_matrix();
    glm::mat4 matrix = proj * view;
    g_shader.UniformMat4("u_matrix", matrix);
    switch (engine.lod) {
        case LOD_0:
            g_globe_0.draw();
            break;
        case LOD_1:
            g_globe_1.draw();
            break;
        case LOD_2:
            g_globe_2.draw();
            break;
    }
}

void process_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin("the_globe", NULL, window_flags);
    std::stringstream ss;
    ss << int(1000*engine.delta) << " ms " << round(1/engine.delta);
    ImGui::Text("delta time: %s fps", ss.str().c_str());

    ImGui::NewLine();
    ImGui::Text("level of quality [l]:");
    ImGui::RadioButton("low", &(engine.lod), LOD_0); ImGui::SameLine();
    ImGui::RadioButton("medium", &(engine.lod), LOD_1); ImGui::SameLine();
    ImGui::RadioButton("high", &(engine.lod), LOD_2);

    ImGui::NewLine();
    if (ImGui::Button("reset camera [c]")) {
        g_camera.reset();
    }
    ImGui::SameLine();
    if (ImGui::Button("exit [q]")) {
        g_window.SetShouldClose(true);
    }

    ImGui::End();
}

void render_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanup_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    g_window.MakeContextCurrent();
    setup_ImGui();
    reset_opengl_settings();
    g_shader.Bind();

    while (!g_window.ShouldClose()) {
        PROFILER_RESET;

        PROFILE(compute_delta());
        PROFILE(process_gui());
        PROFILE(process_orbit_momentum());
        PROFILE(g_camera.set_aspect(g_window.GetAspect()));

        // render calls
        PROFILE(GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
        PROFILE(render_globe());
        PROFILE(render_gui());

        PROFILE(g_window.Update());
    }

    cleanup_gui();

    return 0;
}
