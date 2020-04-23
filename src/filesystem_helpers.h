#pragma once
#include <filesystem>
#include <string>

std::filesystem::path make_file_path(const std::filesystem::path &path, const std::string &file_name);

time_t file_age_seconds(const std::filesystem::path& file_path);
