#pragma once

#include <string>
#include <csignal>

extern volatile sig_atomic_t running;

std::string     to_upper_string(const std::string& s);
bool            is_unsigned_int(const std::string& s);
void            signal_handler(int sig);