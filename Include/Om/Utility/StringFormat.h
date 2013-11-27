#pragma once

#include <string>
#include <vector>

std::string StringFormat(const char *fmt, ...);
void StringFormat(std::string &, const char *fmt, ...);
void StringFormat(std::vector<char> &, const char *fmt, ...);

//EOF
