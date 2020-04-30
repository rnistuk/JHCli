#pragma once
#include "region.h"
#include <map>
#include <iostream>

void handle_provinces(std::map<std::string, regions> &covid19_data);

void handle_countries(std::map<std::string, regions> &covid19_data);

template <class T>
void
print_strings(const T &c) {
    std::for_each(c.begin(), c.end(), [](const auto &t){std::cout << t << "\n";});
}