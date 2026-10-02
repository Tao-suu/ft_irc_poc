#include "utils.h"

#include <algorithm>

std::string     to_upper_string(const std::string& str) {
    std::string new_one = str;
    for (size_t i = 0; i < new_one.size(); i++) {
        new_one[i] = ::toupper(static_cast<unsigned char>(new_one[i]));
    }
    return new_one;
}