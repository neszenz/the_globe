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
        Globe(unsigned n_samples_equator);
        ~Globe();

        void draw() const;
        void draw_low_quality() const;
    private:
        vertex_array_ids m_vai;
        vertex_array_ids m_vai_low;
};

#endif//GLOBE_HPP
