#include "src/johns_hopkins.h"
#include "src/data_parser.h"
#include "src/filesystem_helpers.h"
#include <iostream>
#include <map>
#include <filesystem>
#include <set>

namespace {
    const char *JOHNS_HOPKINS_DATA_URL{
            "https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/"};
    const char *GLOBAL_CONFIRMED_FILE{"time_series_covid19_confirmed_global.csv"};
    const char *GLOBAL_DEATHS_FILE{"time_series_covid19_deaths_global.csv"};
    const char *GLOBAL_RECOVERED_FILE{"time_series_covid19_recovered_global.csv"};

    std::filesystem::path
    temporary_directory(const std::string &sub_dir_name) {
        std::filesystem::path trg_path{make_file_path(std::filesystem::temp_directory_path(), sub_dir_name)};
        if (!std::filesystem::exists(trg_path)) {
            std::filesystem::create_directories(trg_path);
        }
        return trg_path;
    }

    template <class T>
    void
    print_strings(const T &c) {
        std::for_each(c.begin(), c.end(), [](const auto &t){std::cout << t << "\n";});
    }
}

int
process_arguments(int argc, char *argv[], std::map<std::string, std::vector<region_data>> covid19_data) {
    if (argc > 1) {
        const std::string cmd{argv[1]};
        if (cmd == "provinces" || cmd == "states") {
            std::set<std::string> provinces;
            std::for_each(covid19_data.begin(), covid19_data.end(), [&provinces](const auto &p) {
                std::for_each(p.second.begin(), p.second.end(), [&provinces](const auto &r){
                    if (!r.province.empty()) {
                        provinces.emplace(r.province);
                    }
                });
            });
            print_strings(provinces);
        } else if (cmd == "countries" || cmd == "regions") {
            std::set<std::string> countries;
            std::for_each(covid19_data.begin(), covid19_data.end(), [&countries](const auto &p) {
                std::for_each(p.second.begin(), p.second.end(), [&countries](const auto &r){
                    if (!r.country.empty()) {
                        countries.emplace(r.country);
                    }
                });
            });
            print_strings(countries);
        }
        std::cout << std::endl;
    }
    return 0;
}

int
main(int argc, char *argv[]) {
    // Set up source paths
    std::filesystem::path src_url{JOHNS_HOPKINS_DATA_URL};
    std::vector<std::string> src_files{GLOBAL_CONFIRMED_FILE, GLOBAL_DEATHS_FILE, GLOBAL_RECOVERED_FILE};

    // set up target directory
    std::filesystem::path trg_path{temporary_directory(std::filesystem::path(argv[0]).filename())};

    if (argc==1) {
        float max_age{0};
        for (const auto& p : std::filesystem::directory_iterator(trg_path)) {
            const auto age_days{float(file_age_seconds(p)) / (24.0 * 3600.0)};
            std::cout << p.path().string() << " is " << age_days << " days old" << std::endl;
            max_age = age_days > max_age ? age_days : max_age;
        }
    }

    update_from_john_hopkins_if_required(src_url, trg_path, src_files);

    // todo move this next secton into process_arguments
    std::map<std::string, regions> covid19_data;
    std::for_each(src_files.begin(), src_files.end(), [&covid19_data, trg_path](const auto &filename) {
        auto filepath{make_file_path(trg_path, filename)};

        std::ifstream covid_data_stream(filepath.string().c_str());
        auto lines{parse_stream_to_lines(covid_data_stream)};
        auto title_string{pop_front(lines)};
        auto titles{parse_titles(title_string)};
        auto regions{parse_regions(titles, lines)};
        covid19_data[filename] = regions;
    });

    return process_arguments(argc, argv, covid19_data);
}
