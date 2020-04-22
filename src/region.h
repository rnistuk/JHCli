#pragma once

#include <ctime>
#include <string>
#include <deque>
#include <vector>

struct population_data {
    tm time;
    uint64_t people;
};

struct region_data {
    std::string country;
    std::string province;
    std::vector<population_data> data;
};

typedef std::deque<std::string> data_lines;
