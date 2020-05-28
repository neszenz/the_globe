#include "elevation_sampler.hpp"

#include <exception>
#include <iostream>

#include "stb_image.h"

// hard coded meta data of mappings must never be changed
#define SEA_LEVEL         0
#define MAX_ABOVE_SL   9000
#define MIN_BELOW_SL -11000

#define N_PIXEL_BYTES STBI_rgb
#define R_OFFSET 0
#define G_OFFSET 1
#define B_OFFSET 2

#define IMAGE_RESOLUTION_X 1350
#define IMAGE_RESOLUTION_Y 1350
#define N_IMAGES_X 8
#define N_IMAGES_Y 4
#define N_IMAGES (N_IMAGES_X * N_IMAGES_Y)
#define SAMPLE_COUNT_X (N_IMAGES_X * IMAGE_RESOLUTION_X)
#define SAMPLE_COUNT_Y (N_IMAGES_Y * IMAGE_RESOLUTION_Y)

std::string name_from_ids(int i_row, int i_col) {
    std::string name;

    switch (i_row) {
        case 0:
            name += "90N";
            break;
        case 1:
            name += "45N";
            break;
        case 2:
            name += "00N";
            break;
        case 3:
            name += "45S";
            break;
        default:
            throw std::runtime_error("name_from_ids() i_row error");
    };

    switch (i_col) {
        case 0:
            name += "180W";
            break;
        case 1:
            name += "135W";
            break;
        case 2:
            name += "090W";
            break;
        case 3:
            name += "045W";
            break;
        case 4:
            name += "000E";
            break;
        case 5:
            name += "045E";
            break;
        case 6:
            name += "090E";
            break;
        case 7:
            name += "135E";
            break;
        default:
            throw std::runtime_error("name_from_ids() i_col error");
    };

    name += ".jpg";

    return name;
}

int elevation_from_rgb(unsigned r, unsigned g, unsigned b) {
    //TODO
    return 0;
}

float factor_from_elevation(int elevation) {
    if (elevation == SEA_LEVEL) {
        return 0.0f;
    } else if (elevation < SEA_LEVEL) {
        return float(elevation) / std::abs(MIN_BELOW_SL);
    } else {
        return float(elevation) / MAX_ABOVE_SL;
    }
}

// class implementation  - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +
Elevation_Sampler::Elevation_Sampler(const std::string& directory) {
    // preallocate sample grid
    m_samples = sample_rows_t(SAMPLE_COUNT_Y);
    for (sample_row_t& row : m_samples) {
        row = sample_row_t(SAMPLE_COUNT_X, glm::vec3(0.0f));
    }

    for (int i_image = 0; i_image < N_IMAGES; ++i_image) {
        std::cout << "\rprocess elevation data... ";
        std::cout << 100 * i_image / N_IMAGES << "%";
        std::cout.flush();

        int i_row = i_image / N_IMAGES_X;
        int i_col = i_image % N_IMAGES_X;
        std::string path = directory + name_from_ids(i_row, i_col);

        int x, y, n;
        unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, N_PIXEL_BYTES);
        if (data == nullptr)
            throw std::runtime_error("Elevation_Sampler() stbi_load error");
        assert(x == IMAGE_RESOLUTION_X && y == IMAGE_RESOLUTION_Y);

        int x_offset = i_col * IMAGE_RESOLUTION_X;
        int y_offset = i_row * IMAGE_RESOLUTION_Y;
        this->parse_image_data(data, x, y, x_offset, y_offset);
        stbi_image_free(data);
    }
    std::cout << "\rprocess elevation data... 100%" << std::endl;
}

float Elevation_Sampler::at(float latitude, float longitude, float size) {
    //TODO
    return -this->sample_from_coords(latitude, longitude).b;
}

void Elevation_Sampler::parse_image_data(unsigned char* data, int X, int Y, int x_offset, int y_offset) {
    assert(x_offset + X <= SAMPLE_COUNT_X);
    assert(y_offset + Y <= SAMPLE_COUNT_Y);

    int n_pixels = X*Y;
    for (int i = 0; i < n_pixels*N_PIXEL_BYTES; i += N_PIXEL_BYTES) {
        int i_pixel = i / N_PIXEL_BYTES;
        int y = i_pixel / X;
        int x = i_pixel % X;

        unsigned r = data[i + R_OFFSET];
        unsigned g = data[i + G_OFFSET];
        unsigned b = data[i + B_OFFSET];

        glm::vec3 rgb(float(r)/255, float(g)/255, float(b)/255);
        m_samples.at(y+y_offset).at(x+x_offset) = rgb;
    }
}

glm::vec3 Elevation_Sampler::sample_from_coords(float latitude, float longitude) {
    unsigned y = SAMPLE_COUNT_Y * (1.0f - (latitude + M_PI/2) / M_PI);
    unsigned x = SAMPLE_COUNT_X * (longitude + M_PI) / (2*M_PI);
    x = std::min(unsigned(SAMPLE_COUNT_X-1), x);
    y = std::min(unsigned(SAMPLE_COUNT_Y-1), y);
    return m_samples.at(y).at(x);
}
