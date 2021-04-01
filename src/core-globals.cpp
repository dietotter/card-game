#include "core-globals.h"

#include <ctime>

namespace nik::glob {

    std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::mutex g_mutex;

}