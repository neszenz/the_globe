#include "globe.hpp"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "gl_utils.h"

#define SAMPLE_COLOR_0 glm::vec3(0.9f, 0.9f, 0.9f)
#define SAMPLE_COLOR_1 glm::vec3(1.1f, 0.1f, 1.1f)

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

    return {vao, vbo, ebo, elements.size()};
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

vertex_array_ids create_sample_2d(float size) {
    assert(size > 0.0f);
    float r = SAMPLE_COLOR_0.r;
    float g = SAMPLE_COLOR_0.g;
    float b = SAMPLE_COLOR_0.b;
    vertex_data_t vertices = {
        // position               // color
         0*size,  0*size, 0.0,    r, g, b,
        -3*size, -3*size, 0.0,    r, g, b,
         0*size, -2*size, 0.0,    r, g, b,
         3*size, -3*size, 0.0,    r, g, b,
         2*size,  0*size, 0.0,    r, g, b,
         3*size,  3*size, 0.0,    r, g, b,
         0*size,  2*size, 0.0,    r, g, b,
        -3*size,  3*size, 0.0,    r, g, b,
        -2*size,  0*size, 0.0,    r, g, b,
    };
    element_data_t elements = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 1,
    };

    return create_vertex_array(vertices, elements);
}

vertex_array_ids create_sample_3d(float size, float radius, float dent) {
    assert(size > 0.0f);
    assert(radius > 0.0f);
    assert(dent > 0.0f && dent < radius);

    float r0 = SAMPLE_COLOR_0.r;
    float g0 = SAMPLE_COLOR_0.g;
    float b0 = SAMPLE_COLOR_0.b;
    float r1 = SAMPLE_COLOR_1.r;
    float g1 = SAMPLE_COLOR_1.g;
    float b1 = SAMPLE_COLOR_1.b;
    float i_factor = dent / radius;
    float ri = i_factor*r1 + (1.0f-i_factor)*r0;
    float gi = i_factor*g1 + (1.0f-i_factor)*g0;
    float bi = i_factor*b1 + (1.0f-i_factor)*b0;

    vertex_data_t vertices = {
        // position                        // color
         0*size,  0*size, radius-dent,     ri, gi, bi,
        -3*size, -3*size, radius,          r0, g0, b0,
         0*size, -2*size, radius,          r0, g0, b0,
         3*size, -3*size, radius,          r0, g0, b0,
         2*size,  0*size, radius,          r0, g0, b0,
         3*size,  3*size, radius,          r0, g0, b0,
         0*size,  2*size, radius,          r0, g0, b0,
        -3*size,  3*size, radius,          r0, g0, b0,
        -2*size,  0*size, radius,          r0, g0, b0,
            0.0,     0.0,    0.0,          r1, g1, b1,
    };
    element_data_t elements = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 1,

        9, 2, 1,
        9, 3, 2,
        9, 4, 3,
        9, 5, 4,
        9, 6, 5,
        9, 7, 6,
        9, 8, 7,
        9, 1, 8,
    };

    return create_vertex_array(vertices, elements);
}

void draw_vertex_array(vertex_array_ids vai) {
    GL(glBindVertexArray(vai.vao));
    GL(glDrawElements(GL_TRIANGLES, vai.n_elements, GL_UNSIGNED_INT, (void *)0));
    GL(glBindVertexArray(0));
}

void destroy_vertex_array(vertex_array_ids vai) {
    GL(glDeleteBuffers(1, &vai.ebo));
    GL(glDeleteBuffers(1, &vai.vbo));
    GL(glDeleteVertexArrays(1, &vai.vao));
}

// class implementation  - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +
Globe::Globe(unsigned n_samples_equator, float radius) : m_n_samples_equator(n_samples_equator) {
    assert(n_samples_equator >= 4);
    assert(radius > 0.0f);
    float size = 0.71*radius / n_samples_equator; // deduced from testing
    m_n_rings = 0.185*radius / size; // deduced from testing
    m_vai = create_sample_3d(size, radius, 0.2f*radius);

    std::cout << "globe created" << std::endl;
}

Globe::~Globe() {
    destroy_vertex_array(m_vai);
}

void Globe::draw(const Shader& shader, const Camera& camera) const {
    glm::mat4 proj_view = camera.get_proj_matrix() * camera.get_view_matrix();
    glm::mat4 eighth_roll = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.Bind();

    for (unsigned i_ring = 0; i_ring <= m_n_rings; i_ring++) {
        float angle_y = 0.5f*M_PI * i_ring / m_n_rings;
        // samples per ring decreases non-linearly depending on ring's radius
        unsigned n_samples = ceil(m_n_samples_equator * std::abs(sin(angle_y + 0.5f*M_PI)));
        for (unsigned i_sample = 0; i_sample < n_samples; i_sample++) {
            float angle_x = 2.0f*M_PI * i_sample / n_samples;

            //float latitude = angle_y;
            //float longitude = (angle_x < M_PI)? angle_x : -2.0f*M_PI + angle_x;

            // equator and northern hemisphere
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, angle_y, glm::vec3(-1.0f, 0.0f, 0.0f));
            glm::mat4 matrix = proj_view * model * eighth_roll;

            shader.UniformMat4("u_matrix", matrix);
            draw_vertex_array(m_vai);

            // southern hemisphere
            if (i_ring > 0) { // skips the equator to avoid sampling it twice
                model = glm::rotate(model, -2*angle_y, glm::vec3(-1.0f, 0.0f, 0.0f));
                matrix = proj_view * model * eighth_roll;
                shader.UniformMat4("u_matrix", matrix);
                draw_vertex_array(m_vai);
            }
        }
    }
}
