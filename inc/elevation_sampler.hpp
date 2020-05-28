#ifndef ELEVATION_SAMPLER_HPP
#define ELEVATION_SAMPLER_HPP

#include <glm/glm.hpp>

#include <string>

class Elevation_Sampler {
    public:
        Elevation_Sampler(const std::string& directory);

        // returns scale factor [-1.0f, 1.0f]
        float at(float latitude, float longitude, float size);
    private:
};

#endif//ELEVATION_SAMPLER_HPP
