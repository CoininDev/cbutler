#pragma once
#include <core/butlerxml.h>

#include <filesystem>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::cmake {
std::string project_name();
std::vector<std::string> inside_flag(const std::string& flag,
                                     const std::string& line);

std::string cmakelists_str();
stdfs::path cmakelists_path();

class CMakeParser : public butlerxml::ConfigParser {
    CMakeParser() {
        _codeblocks["module"] = R"(
            add_subdirectory(src/<__MOD_NAME__>)
        )";
    }
};

}  // namespace core::cmake