#include "filesystem_helpers.h"

std::filesystem::path
make_file_path(const std::filesystem::path &path, const std::string &file_name) {
    auto filepath{path};
    filepath.append(file_name);
    return filepath;
}

time_t
file_age_seconds(const std::filesystem::path& file_path) {
    std::filesystem::file_time_type file_time{last_write_time(file_path)};
    return difftime(time(nullptr), decltype(file_time)::clock::to_time_t(file_time));
}
