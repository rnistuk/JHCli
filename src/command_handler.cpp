#include "command_handler.h"
#include <set>

void
handle_provinces(std::map<std::string, regions> &covid19_data) {
    std::set<std::string> provinces;
    std::for_each(covid19_data.begin(), covid19_data.end(), [&provinces](const auto &p) {
        std::for_each(p.second.begin(), p.second.end(), [&provinces](const auto &r){
            if (!r.province.empty()) {
                provinces.emplace(r.province + " - " + r.country);
            }
        });
    });
    print_strings(provinces);
}


void
handle_countries(std::map<std::string, regions> &covid19_data) {
    std::map<std::string, std::set<std::string>> countries;
    std::for_each(covid19_data.begin(), covid19_data.end(), [&countries](const auto &p) {
        std::for_each(p.second.begin(), p.second.end(), [&countries](const auto &r) {
            if (!r.country.empty()) {
                countries[r.country].emplace(r.province);
            }
        });
    });

    for (const auto &p : countries) {
        std::cout << p.first;
        if (!p.second.empty()) {
            std::cout << " - ";
            for (const auto& r : p.second) {
                if (!r.empty()) {
                    std::cout << r << ",";
                }
            }
            std::cout << std::endl;
        }
    }
}