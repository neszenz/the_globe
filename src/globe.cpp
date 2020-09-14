#include "globe.hpp"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "elevation_sampler.hpp"
#include "gl_utils.h"

#define ELEVATION_MAPS_DIRECTORY "res/maps/"

#define SAMPLE_COLOR_0 glm::vec3(0.9f, 0.9f, 0.9f)
#define SAMPLE_COLOR_1 glm::vec3(1.1f, 0.1f, 1.1f)

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

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

glm::mat4 offset_from_elevation(float elevation) {
    glm::mat4 offset(1.0f);

    float scale = 1.1f+0.1f*elevation; // assuming elevation >= -1.0
    offset = glm::scale(offset, glm::vec3(1.0f, 1.0f, scale));

    return offset;
}

std::vector<glm::mat4> compute_sample_matrices(const Elevation_Sampler& sampler, unsigned n_samples_equator, unsigned n_rings) {
    std::vector<glm::mat4> sample_matrices;

    glm::mat4 init_roll = glm::rotate(glm::mat4(1.0f), float(M_PI/4), Z_AXIS);
    for (unsigned i_ring = 0; i_ring <= n_rings; i_ring++) {
        float angle_y = M_PI/2 * i_ring / n_rings;
        // samples per ring decreases non-linearly depending on ring's radius
        float ring_factor = std::abs(sin(angle_y + M_PI/2));
        unsigned n_samples = std::max(1u, unsigned(n_samples_equator * ring_factor));
        for (unsigned i_sample = 0; i_sample < n_samples; i_sample++) {
            float angle_x = 2.0f*M_PI * i_sample / n_samples;

            // elevation data
            float latitude_north = angle_y;
            float latitude_south = -angle_y;
            float longitude = (angle_x < M_PI)? angle_x : -2.0f*M_PI + angle_x;
            float size = M_PI/2 * 1 / n_rings; // one angle_y step

            // equator and northern hemisphere
            float elevation = sampler.at(latitude_north, longitude, size);
            glm::mat4 offset = offset_from_elevation(elevation);
            glm::mat4 matrix = glm::mat4(1.0f);
            matrix = glm::rotate(matrix, angle_x, Y_AXIS);
            matrix = glm::rotate(matrix, angle_y, -X_AXIS);
            sample_matrices.push_back(matrix * offset * init_roll);

            // southern hemisphere
            if (i_ring > 0) { // skips the equator to avoid sampling it twice
                elevation = sampler.at(latitude_south, longitude, size);
                offset = offset_from_elevation(elevation);
                matrix = glm::rotate(matrix, -2*angle_y, -X_AXIS);
                sample_matrices.push_back(matrix * offset * init_roll);
            }
        }
    }

    return sample_matrices;
}

// positions and colors verctor of default sample and matrices per sample
vertex_data_t globe_vertices(std::vector<glm::vec3> pos, std::vector<glm::vec3> col, std::vector<glm::mat4> sample_matrices) {
    assert(pos.size() == col.size());

    vertex_data_t vertices;

    for (const glm::mat4& matrix : sample_matrices) {
        for (unsigned i = 0; i < pos.size(); ++i) {
            glm::vec4 p = matrix * glm::vec4(pos.at(i), 1.0f);
            vertices.push_back(p.x);
            vertices.push_back(p.y);
            vertices.push_back(p.z);
            vertices.push_back(col.at(i).r);
            vertices.push_back(col.at(i).g);
            vertices.push_back(col.at(i).b);
        }
    }

    return vertices;
}

element_data_t expand_elements(const element_data_t& elem, unsigned n_samples, unsigned n_vertices) {
    element_data_t elements;

    for (unsigned i_sample = 0; i_sample < n_samples; ++i_sample) {
        for (unsigned e : elem) {
            elements.push_back(e + i_sample * n_vertices);
        }
    }

    return elements;
}

vertex_array_ids create_globe(float size, std::vector<glm::mat4> sample_matrices) {
    assert(size > 0.0f);
    float radius = 1.0f;
    float dent = 0.05f * radius;

    // first, hard coded vertex positions and colors of one sample
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

    std::vector<glm::vec3> positions = {
        glm::vec3( 0*size,  0*size, radius-dent),
        glm::vec3(-3*size, -3*size, radius),
        glm::vec3( 0*size, -2*size, radius),
        glm::vec3( 3*size, -3*size, radius),
        glm::vec3( 2*size,  0*size, radius),
        glm::vec3( 3*size,  3*size, radius),
        glm::vec3( 0*size,  2*size, radius),
        glm::vec3(-3*size,  3*size, radius),
        glm::vec3(-2*size,  0*size, radius),
        glm::vec3(0.0,     0.0,    0.0)
    };

    std::vector<glm::vec3> colors = {
        glm::vec3(ri, gi, bi),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r0, g0, b0),
        glm::vec3(r1, g1, b1)
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

    // second, generate vertex and element data for all samples
    vertex_data_t vertices = globe_vertices(positions, colors, sample_matrices);
    elements = expand_elements(elements, sample_matrices.size(), positions.size());

    return create_vertex_array(vertices, elements);
}

// class implementation  - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +
Globe::Globe(unsigned n_samples_equator) {
    assert(n_samples_equator >= 4);
    float size = 0.71 / n_samples_equator; // deduced from testing
    unsigned n_rings = 0.185 / size; // deduced from testing

    static Elevation_Sampler sampler(ELEVATION_MAPS_DIRECTORY);
    std::vector<glm::mat4> sample_matrices = compute_sample_matrices(sampler, n_samples_equator, n_rings);

    m_vai = create_globe(size, sample_matrices);
}

Globe::~Globe() {
    destroy_vertex_array(m_vai);
}

void Globe::draw() const {
    draw_vertex_array(m_vai);
}
