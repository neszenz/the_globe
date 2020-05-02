#include "profiler.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "util.hpp"

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
    std::cout << earliest_time << " sec\t\tinit_timestamp"<< std::endl;
    for (profile_t p : m_profiles) {
        double duration = p.second.second - p.second.first;

        const std::string& name_str = p.first;
        std::string time_str = string_from_seconds(duration);

        std::cout << time_str << "\t" << name_str << std::endl;
    }
    double total_duration = latest_time - earliest_time;
    std::string time_str = string_from_seconds(total_duration);
    std::cout << time_str << "\ttotal" << std::endl;
}
