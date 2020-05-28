#include "elevation_sampler.hpp"

#include <exception>
#include <iostream>

#include "stb_image.h"

Elevation_Sampler::Elevation_Sampler(const std::string& directory) {
    //TODO
    std::string file = directory + std::string("00N000E.jpg");
    int x,y,n;
    unsigned char *data = stbi_load(file.c_str(), &x, &y, &n, 0);
    if (data == nullptr) {
        throw std::runtime_error("Elevation_Sampler() stbi_load error");
    }
    std::cout << data << std::endl;
    std::cout << x << ", " << y << std::endl;
    std::cout << n << std::endl;
}

float Elevation_Sampler::at(float latitude, float longitude, float size) {
    //TODO
    return 0.0f;
}
