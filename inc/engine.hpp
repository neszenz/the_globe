#ifndef ENGINE_HPP
#define ENGINE_HPP

#define LOD_0 64
#define LOD_1 256
#define LOD_2 1024

struct engine_t {
    double delta = -1.0;

    glm::vec3 momentum = glm::vec3(0.0f);

    int lod = LOD_0;

    struct mouse_t {
        bool is_down = false;
        double xlast = 0.0;
        double ylast = 0.0;
    } mouse;
};

extern struct engine_t engine;

#endif//ENGINE_HPP
