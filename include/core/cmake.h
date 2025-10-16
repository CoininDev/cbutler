#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::cmake {
std::string project_name();
std::vector<std::string> inside_flag(const std::string &flag,
                                     const std::string &line);

std::string cmakelists_str();
stdfs::path cmakelists_path();

} // namespace core::cmake