#pragma once
#include <core/butlerxml.h>

#include <filesystem>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::cmake {
std::string project_name(const stdfs::path& configfile);

std::vector<std::string> inside_flag(const std::string& flag,
                                     const std::string& line);

std::string cmakelists_str();
stdfs::path cmakelists_path();
std::string wrap_flag(const std::string& flag, const std::string& content);

}  // namespace core::cmake