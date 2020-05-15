#include "profiler.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "util.hpp"

std::string string_from_ratio(double partial, double total) {
    char str[32];
    if (total == 0.0) {
        return "  inf%";
    }

    double ratio = 100.0 * partial / total;
    if (ratio == 100.0) {
        sprintf(str, "%.1f%%", ratio);      // "100.0%"
    } else if (ratio >= 10.0) {
        sprintf(str, " %.1f%%", ratio);     // " 10.0%"
    } else if (ratio >= 1.0) {
        sprintf(str, "  %.1f%%", ratio);    // "  1.0%"
    } else {
        sprintf(str, "%.3f%%", ratio);      // "0.001%"
    }

    return str;
}

void Profiler::reset() {
    m_profiles.clear();
}

void Profiler::add_profile(const std::string& name) {
    m_profiles.push_back(std::make_pair(name,
                std::make_pair(glfwGetTime(), -1.0)));
}

void Profiler::stop_latest_profile() {
    m_profiles.back().second.second = glfwGetTime();
}

void Profiler::print_profile() {
    double earliest_time = m_profiles.front().second.first;
    double latest_time = m_profiles.back().second.second;
    double total_duration = latest_time - earliest_time;
    double captured = 0.0;

    std::cout << earliest_time << " sec\t\t\tinit_timestamp"<< std::endl;
    for (profile_t p : m_profiles) {
        double duration = p.second.second - p.second.first;
        captured += duration;

        std::string ratio_str = string_from_ratio(duration, total_duration);
        std::string time_str = string_from_seconds(duration);
        const std::string& name_str = p.first;

        std::cout << ratio_str << "\t" << time_str << "\t" << name_str << std::endl;
    }
    double uncaptured = total_duration - captured;
    std::string uncap_ratio_str = string_from_ratio(uncaptured, total_duration);
    std::string uncap_str = string_from_seconds(uncaptured);
    std::cout << uncap_ratio_str << "\t" << uncap_str << "\tuncaptured" << std::endl;

    std::string time_str = string_from_seconds(total_duration);
    std::cout << "100.0%\t" << time_str << "\ttotal capture scope" << std::endl;
}
