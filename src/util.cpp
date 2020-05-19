#include "util.hpp"

#include <sstream>

std::string string_from_seconds(double time) {
    int ms_time =       1000 * time;
    int ns_time =    1000000 * time - (1000 * ms_time);
    int ps_time = 1000000000 * time - (1000000 * ms_time) - (1000 * ns_time);

    std::stringstream ss;
    ss << ms_time << " ms ";
    ss << ns_time << " ns ";
    ss << ps_time << " ps";

    return ss.str();
}

//TODO fix
glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size) {
    double x =  (2.0 * screen_vec.x - screen_size.x) / screen_size.x;
    double y = -(2.0 * screen_vec.y - screen_size.y) / screen_size.y;
    double x2y2 = x * x + y * y;

    const double TRACKBALL_RADIUS = 1.0;
    const double rsqr = TRACKBALL_RADIUS * TRACKBALL_RADIUS;

    glm::vec3 v3D;
    v3D.x = x;
    v3D.y = y;
    if (x2y2 < 0.5 * rsqr) {
        v3D.z = std::sqrt(rsqr - x2y2);
    } else {
        v3D.z = 0.5 * rsqr / std::sqrt(x2y2);
    }

    return glm::normalize(v3D);
}

