#pragma once
#include <filesystem>
#include <string>

namespace core::process {

int run(const std::string& command, const std::filesystem::path& path);

}