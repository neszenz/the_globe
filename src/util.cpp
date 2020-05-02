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
