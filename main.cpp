#include "src/johns_hopkins.h"

#include "src/data_parser.h"

#include <iostream>
#include <map>
#include <filesystem>
#include <set>

const char *JOHNS_HOPKINS_DATA_URL{
        "https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/"};
const char *GLOBAL_CONFIRMED_FILE{"time_series_covid19_confirmed_global.csv"};
const char *GLOBAL_DEATHS_FILE{"time_series_covid19_deaths_global.csv"};
const char *GLOBAL_RECOVERED_FILE{"time_series_covid19_recovered_global.csv"};

std::filesystem::path
temporary_directory(const std::string &data_dir_name) {
    std::filesystem::path app_path{data_dir_name};
    std::filesystem::path trg_path{std::filesystem::temp_directory_path()};
    trg_path.append(app_path.filename().c_str());
    if (!std::filesystem::exists(trg_path)) {
        std::filesystem::create_directories(trg_path);
    }
    return trg_path;
}


void
process_arguments(int argc, char *argv[], std::map<std::string, std::vector<region_data>> covid19_data) {
    if (argc > 1) {
        const std::string cmd{argv[1]};
        if (cmd == "provinces" || cmd == "states") {
            std::set<std::string> provinces;
            std::for_each(covid19_data.begin(), covid19_data.end(), [&provinces](const auto &p) {
                for (const auto &r : p.second) {
                    if (!r.province.empty()) {
                        provinces.emplace(r.province);
                    }
                }
            });
            std::for_each(provinces.begin(), provinces.end(), [](const auto &p) {
                std::cout << p << "\n";
            });
        }
        std::cout << std::endl;
    }
}


int
main(int argc, char *argv[]) {
    // Set up source paths
    std::filesystem::path src_url{JOHNS_HOPKINS_DATA_URL};
    std::vector<std::string> src_files{GLOBAL_CONFIRMED_FILE, GLOBAL_DEATHS_FILE, GLOBAL_RECOVERED_FILE};

    // set up target directory
    std::filesystem::path trg_path{temporary_directory(argv[0])};

    update_from_john_hopkins_if_required(src_url, trg_path, src_files);

    std::map<std::string, std::vector<region_data>> covid19_data;
    std::for_each(src_files.begin(), src_files.end(), [&covid19_data, trg_path](const auto &filename) {
        auto filepath{trg_path};
        filepath.append(filename);

        std::ifstream covid_data_stream(filepath.string().c_str());
        auto lines{parse_stream_to_lines(covid_data_stream)};

        auto title_string{lines.front()};
        lines.pop_front();

        auto titles{parse_titles(title_string)};
        auto regions{parse_regions(titles, lines)};

        covid19_data[filename] = regions;
    });


    process_arguments(argc, argv, covid19_data);






/*

    for (const auto &p : covid19_data) {
        std::cout << p.first << " : " << p.second.size() << "\n";

        std::vector<region_data> canada;
        std::copy_if(p.second.begin(), p.second.end(), std::back_inserter(canada),
                     [](const auto &r) {
                         return r.country == "Canada";
                     });


        std::cout << canada.size() << std::endl;
    }*/

    return 0;
}
