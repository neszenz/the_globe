#ifndef ENGINE_HPP
#define ENGINE_HPP

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

struct engine_t {
    double delta = -1.0;
    glm::vec3 momentum = glm::vec3(0.0f);
    struct mouse_t {
        bool is_down = false;
        double xlast = 0.0;
        double ylast = 0.0;
    } mouse;
};

extern struct engine_t engine;

#endif//ENGINE_HPP
