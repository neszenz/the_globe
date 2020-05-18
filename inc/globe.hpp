#ifndef GLOBE_HPP
#define GLOBE_HPP

#include <glm/glm.hpp>

struct vertex_array_ids {
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
};

class Globe {
    public:
        Globe(int n_samples);
        ~Globe();

        glm::mat4 get_model_matrix() const;
        void reset_model_matrix();

        // animated rotational momentum, fps-independent via delta time
        void add_momentum(const glm::vec3& impulse);
        void add_momentum(float x, float y, float z);
        void process_momentum(double delta);
        void reset_momentum();

        void draw() const;
    private:
        vertex_array_ids m_vai;

        glm::mat4 m_model = glm::mat4(1.0f);
        glm::vec3 m_momentum = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif//GLOBE_HPP
