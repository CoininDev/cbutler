#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace core::fs {

void ensure_dir(const std::filesystem::path &dir);
void remove_dir(const std::filesystem::path &dir);
void require_file(const std::filesystem::path &file);
std::string read_to_string(const std::filesystem::path &path);
std::vector<std::string> read_lines(const std::filesystem::path &path);
std::string join_lines(const std::vector<std::string> &lines);
std::tuple<std::string, int> find_line_with(const std::filesystem::path &path,
                                            const std::string &exp);
void overwrite(const std::filesystem::path &path, const std::string &content);

} // namespace core::fs
