#ifndef ELEVATION_SAMPLER_HPP
#define ELEVATION_SAMPLER_HPP

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Elevation_Sampler {
    public:
        Elevation_Sampler(const std::string& directory);

        /** averaged elevation at specified region
         * latitude and longitude in radians
         * size: square's side length as radian of latitude
         * return: elevation sample factor [-1.0, 1.0] or 2.0
         **/
        float at(float latitude, float longitude, float size);
    private:
        typedef std::vector<float> sample_row_t;
        typedef std::vector<sample_row_t> sample_rows_t;

        sample_rows_t m_samples;

        void parse_image_data(unsigned char* data, int X, int Y, int x_offset, int y_offset);
        float sample_from_coords(float latitude, float longitude);
};

#endif//ELEVATION_SAMPLER_HPP
