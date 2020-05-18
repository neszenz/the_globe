#include "globe.hpp"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "gl_utils.h"

typedef std::vector<float> vertex_data_t;
typedef std::vector<unsigned> element_data_t;

vertex_array_ids create_vertex_array(const vertex_data_t& vertices,
                                     const element_data_t& elements) {
    unsigned v_size = sizeof(vertices[0]) * vertices.size();
    unsigned i_size = sizeof(elements[0]) * elements.size();

    unsigned vao;
    GL(glGenVertexArrays(1, &vao));
    GL(glBindVertexArray(vao));

    unsigned vbo;
    GL(glGenBuffers(1, &vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL(glBufferData(GL_ARRAY_BUFFER, v_size, &vertices[0], GL_STATIC_DRAW));

    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0));
    GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float))));
    GL(glEnableVertexAttribArray(0));
    GL(glEnableVertexAttribArray(1));

    unsigned ebo;
    GL(glGenBuffers(1, &ebo));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, &elements[0], GL_STATIC_DRAW));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    return {vao, vbo, ebo};
}

vertex_array_ids create_color_cube() {
    vertex_data_t vertices = {
        // position         // color
         1.0,  1.0,  1.0,   1.0, 1.0, 1.0,  // 0 front top right
         1.0, -1.0,  1.0,   1.0, 0.0, 1.0,  // 1 front bottom right
        -1.0, -1.0,  1.0,   0.0, 0.0, 1.0,  // 2 front bottom left
        -1.0,  1.0,  1.0,   0.0, 1.0, 1.0,  // 3 front top left

         1.0,  1.0, -1.0,   1.0, 1.0, 0.0,  // 4 back top right
         1.0, -1.0, -1.0,   1.0, 0.0, 0.0,  // 5 back bottom right
        -1.0, -1.0, -1.0,   0.0, 0.0, 0.0,  // 6 back bottom left
        -1.0,  1.0, -1.0,   0.0, 1.0, 0.0   // 7 back top left
    };
    element_data_t elements = {
        // front side
        0, 3, 1,
        3, 2, 1,
        // back side
        7, 4, 6,
        4, 5, 6,
        // left side
        3, 7, 2,
        7, 6, 2,
        // right side
        4, 0, 5,
        0, 1, 5,
        // bottom side
        1, 2, 5,
        2, 6, 5,
        // top side
        4, 7, 0,
        7, 3, 0
    };

    return create_vertex_array(vertices, elements);
}

void destroy_vertex_array(vertex_array_ids vai) {
    GL(glDeleteBuffers(1, &vai.ebo));
    GL(glDeleteBuffers(1, &vai.vbo));
    GL(glDeleteVertexArrays(1, &vai.vao));
}

// class implementation  - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +
Globe::Globe(int n_samples) {
    vertex_data_t vertices = {
        // position         // color
        -1.0, -1.0,  0.0,    1.0, 0.0, 0.0,
         1.0, -1.0,  0.0,    0.0, 1.0, 0.0,
         0.0,  1.0,  0.0,    0.0, 0.0, 1.0
    };
    element_data_t elements = {
        0, 1, 2
    };

    /* m_vai = create_vertex_array(vertices, elements); */
    m_vai = create_color_cube();

    std::cout << "globe created" << std::endl;
}

Globe::~Globe() {
    destroy_vertex_array(m_vai);
}

glm::mat4 Globe::get_model_matrix() const {
    return m_model;
}
void Globe::reset_model_matrix() {
    m_model = glm::mat4(1.0f);
}

void Globe::add_momentum(const glm::vec3& impulse) {
    m_momentum += impulse;
}
void Globe::add_momentum(float x, float y, float z) {
    m_momentum += glm::vec3(x, y, z);
}
void Globe::process_momentum(double delta) {
    float speed = 0.1f;
    float smoothing = delta * (1.0f / speed);
    glm::vec3 curr_momentum = smoothing * m_momentum;
    m_model = glm::rotate(m_model, curr_momentum.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m_model = glm::rotate(m_model, curr_momentum.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m_model = glm::rotate(m_model, curr_momentum.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_momentum -= curr_momentum;
}
void Globe::reset_momentum() {
    m_momentum = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Globe::draw() const {
    GL(glBindVertexArray(m_vai.vao));
    GL(glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void *)0));
    GL(glBindVertexArray(0));
}
