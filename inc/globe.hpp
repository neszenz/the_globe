#ifndef GLOBE_HPP
#define GLOBE_HPP

#include <vector>

struct vertex_array_ids {
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
};

class Globe {
    public:
        Globe(int n_samples);
        ~Globe();

        void draw() const;
    private:
        vertex_array_ids m_vai;
};

#endif//GLOBE_HPP
