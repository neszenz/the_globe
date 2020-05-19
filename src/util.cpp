#include "util.hpp"

#include <assert.h>
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

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size) {
    // Ken Shoemake's ArcBall sphere mapping
    glm::vec2 circle_center(screen_size.x/2, screen_size.y/2);
    double radius = 400.0;

    double x =  (screen_vec.x - circle_center.x) / radius;
    double y = -(screen_vec.y - circle_center.y) / radius;
    double z = 0.0;
    double r = x*x + y*y;

    if (1.0 - r <= 0.0) { // modified condition fixes rounding issue for sqrt
        double s = 1.0 / std::sqrt(r);
        x = s * x;
        y = s * y;
    } else {
        assert(1.0 - r > 0.0); // should be redundant due to modified condition
        z = std::sqrt(1.0 - r);
    }

    return glm::vec3(x, y, z);
}

