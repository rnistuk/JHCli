#pragma once

#include <string>
#include <filesystem>

std::string john_hopkins_data_to_file(const std::string &src_file, const std::string &filename);

void
update_from_john_hopkins_if_required(const std::filesystem::path &src_url, const std::filesystem::path &trg_path,
                                     const std::vector<std::string> &src_files);
