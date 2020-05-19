#ifndef UTIL_HPP
#define UTIL_HPP

#include <glm/glm.hpp>
#include <string>

std::string string_from_seconds(double time);

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size);

#endif//UTIL_HPP
