#include "utils.h"

#include <algorithm>
#include <cstring>

std::string     to_upper_string(const std::string& str) {
    std::string new_one = str;
    for (size_t i = 0; i < new_one.size(); i++) {
        new_one[i] = ::toupper(static_cast<unsigned char>(new_one[i]));
    }
    return new_one;
}

bool            is_unsigned_int(const std::string& str) {
    if (str.size() > 10 || str.empty()) return false;
    for (size_t i = 0; i < str.size(); i++) if (!::isdigit(str[i])) return false;
    if (str.size() < 10) return true;
    if (::strcmp(str.c_str(), "2147483647") > 0) return false;
    return true;
}