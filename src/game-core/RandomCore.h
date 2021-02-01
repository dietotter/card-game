#pragma once

#include <ctime>
#include <random>

namespace rnd {

    // TODO if I have some config class, this could probably be in there
    class RandomCore
    {
    public:
        static std::mt19937 mt;
    };

    std::mt19937 RandomCore::mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

}