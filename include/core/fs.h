#pragma once

#include <filesystem>
#include <string>

namespace core::fs {

void ensure_dir(const std::filesystem::path& dir);
void remove_dir(const std::filesystem::path& dir);
void require_file(const std::filesystem::path& file);
std::string read_to_string(const std::filesystem::path& path);
void overwrite(const std::filesystem::path& path, const std::string& content);

}  // namespace core::fs
