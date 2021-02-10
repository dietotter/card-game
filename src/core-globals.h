#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <random>

namespace nik::glob {

    // mersenne twister for random stuff
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

}