#include "data_parser.h"
#include <iostream>
#include <sstream>

tm
string_to_date(const std::string &date) {
    tm tm;
    std::memset(&tm, 0, sizeof(struct tm));
    strptime(date.c_str(), "%D", &tm);
    return tm;
}

std::vector<tm>
dates_from_titles(const std::vector<std::string> &titles) {
    std::vector<tm> dates;
    for (int i{4}; i < titles.size(); ++i) {
        const tm dt{string_to_date(titles[i])};
        if (dt.tm_year == 0) {
            std::cerr << "Empty date at i: " << i << std::endl;
        }
        dates.emplace_back(dt);
    }
    return dates;
}

std::string
re_delimit_to_pipes(const std::string &d) {
    // convert ',' to '|' if they are not inside of quotes
    std::string delimitedData;
    bool inQuotes = false;
    std::transform(d.begin(), d.end(), std::back_inserter(delimitedData),
                   [&inQuotes](const char &c) {
                       if (c == '"') {
                           inQuotes = !inQuotes;
                       }
                       return (',' == c && !inQuotes) ? '|' : c;
                   });
    return delimitedData;
}

data_lines
parse_stream_to_lines(std::ifstream &data_stream) {
    data_lines lines;
    while (data_stream) {
        std::string line;
        std::getline(data_stream, line);
        if (line.empty()) {
            std::cerr << "line " << lines.size() << " is empty, ignoring" << std::endl;
            continue;
        }
        lines.emplace_back(line);
    }
    return lines;
}

std::vector<std::string>
parse_titles(const std::string &title_string) {
    std::vector<std::string> titles;
    std::stringstream ss{title_string};
    while (ss) {
        std::string t;
        std::getline(ss, t, '|');
        if (t.empty()) {
            std::cerr << "Title part empty string, ignoring" << std::endl;
            continue;
        }
        titles.emplace_back(t);
    }
    return titles;
}

std::pair<std::vector<std::string>, std::vector<uint64_t>>
data_line_to_parts(const std::string &l) {
    std::vector<std::string> info;
    std::vector<uint64_t> infections;
    std::stringstream ss{l};
    int i{0};
    while (ss) {
        std::string s;
        getline(ss, s, '|');
        if (i < 4) {
            if (i == 0 || i == 1) {
                info.emplace_back(s);
            }
        } else {
            if (!s.empty()) {
                try {
                    infections.emplace_back(std::stoi(s.c_str()));
                } catch (const std::exception &e) {
                    std::cerr << "\ti" << i << " err: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Empty string at index " << i << std::endl;
            }
        }
        ++i;
    }
    return std::make_pair(info, infections);
}

regions
parse_regions(const std::vector<std::string> &titles, data_lines lines) {
    std::vector<tm> dates{dates_from_titles(titles)};

    regions regions;
    for (const auto &l : lines) {
        auto parts = data_line_to_parts(l);

        region_data rd;
        rd.province = parts.first[0];
        rd.country = parts.first[1];

        for (int i{0}; i < dates.size(); ++i) {
            rd.data.emplace_back(population_data{dates[i], parts.second[i]});
        }

        regions.emplace_back(rd);
    }
    return regions;
}
