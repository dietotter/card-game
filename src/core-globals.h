#pragma once

#include <SFML/Graphics.hpp>

#include <mutex>
#include <random>

namespace nik::glob {

    // mersenne twister for random stuff
    extern std::mt19937 mt;
    // global mutex
    extern std::mutex g_mutex;

}