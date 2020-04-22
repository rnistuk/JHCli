#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <sstream>

#include "src/data_parser.h"
#include "src/johns_hopkins.h"
#include "src/region.h"



void
update_from_john_hopkins_if_required(const std::filesystem::path& src_url, const std::filesystem::path& trg_path, const std::vector<std::string>& src_files) {
    std::for_each(src_files.begin(), src_files.end(),[src_url,trg_path](const auto& file_name) {
        std::filesystem::path source{src_url};
        std::filesystem::path target{trg_path};
        source.append(file_name);
        target.append(file_name);

        if (std::filesystem::exists(target)) {
            std::filesystem::file_time_type file_time {last_write_time(target)};
            double difference_sec = difftime(time(nullptr), decltype(file_time)::clock::to_time_t(file_time)); //seconds to day
            if (difference_sec < (24*3600)) {
                return;
            }
            std::cerr << "Data is over one day old, reloading from John Hopkins." << std::endl;
        } else {
            std::cerr << "Downloading " << file_name << " from John Hopkins." << std::endl;
        }
        john_hopkins_data_to_file(source.string(), target.string() );
    });
}

int
main(int argc, char *argv[]) {
    // Set up source paths
    std::filesystem::path src_url{"https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/csse_covid_19_time_series/"};
    std::vector<std::string> src_files{"time_series_covid19_confirmed_global.csv", "time_series_covid19_deaths_global.csv",  "time_series_covid19_recovered_global.csv" };

    // set up target directory
    std::filesystem::path app_path{argv[0]};
    std::filesystem::path trg_path{std::filesystem::temp_directory_path()};
    trg_path.append(app_path.filename().c_str());

    if(!std::filesystem::exists(trg_path)) {
        std::filesystem::create_directories(trg_path);
    }

    update_from_john_hopkins_if_required(src_url, trg_path, src_files);

    // load files to datastructures
    std::map<std::string, std::vector<region_data>> covid19_data;
    std::for_each(src_files.begin(), src_files.end(), [&covid19_data, trg_path](const auto& filename) {
        auto filepath{trg_path};
        filepath.append(filename);
        std::ifstream covid_data_stream(filepath.string());
        auto lines{parse_stream_to_lines(covid_data_stream)};
        auto title_string{lines.front()};
        lines.pop_front();
        auto titles{parse_titles(title_string)};
        auto regions{parse_regions(titles, lines)};
        covid19_data[filename] = regions;
    });

    for (const auto& p : covid19_data) {
        std::cout << p.first << " : " << p.second.size() << "\n";

        std::vector<region_data> canada;
        std::copy_if(p.second.begin(), p.second.end(), std::back_inserter(canada),
                     [](const auto &r) {
                         return r.country == "Canada";
                     });


        std::cout << canada.size() << std::endl;
    }

    return 0;
}
