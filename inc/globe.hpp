#ifndef GLOBE_HPP
#define GLOBE_HPP

#include <glm/glm.hpp>

#include "camera.hpp"
#include "shader.h"

struct vertex_array_ids {
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
    unsigned long n_elements;
};

class Globe {
    public:
        Globe(unsigned n_samples_equator, float radius = 1.0f);
        ~Globe();

        void draw(const Shader& shader, const Camera& camera) const;
    private:
        unsigned m_n_samples_equator;
        unsigned m_n_rings;
        vertex_array_ids m_vai;
};

#endif//GLOBE_HPP
