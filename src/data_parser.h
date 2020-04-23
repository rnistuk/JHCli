#pragma once

#include "region.h"

#include <string>
#include <vector>
#include <fstream>

template <class T>
auto
pop_front(T &c) {
    auto e{c.front()};
    c.pop_front();
    return e;
};

tm string_to_date(const std::string &date);

std::vector<tm> dates_from_titles(const std::vector<std::string> &titles);

std::string re_delimit_to_pipes(const std::string &d);

data_lines parse_stream_to_lines(std::ifstream &data_stream);

std::vector<std::string> parse_titles(const std::string &title_string);

regions parse_regions(const std::vector<std::string> &titles, data_lines lines);
