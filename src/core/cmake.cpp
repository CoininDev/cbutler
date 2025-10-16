#include <core/cmake.h>
#include <core/fs.h>

#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::cmake {

std::string project_name() {
    if (!stdfs::exists("CMakeLists.txt"))
        return "";
    auto [line_content, line] =
        core::fs::find_line_with("CMakeLists.txt", "project(");
    if (line == -1) {
        std::cerr << "Ocorreu um erro ao buscar por nome do projeto."
                  << std::endl;
        return "";
    }
    return inside_flag("project", line_content)[0];
}

std::vector<std::string> inside_flag(const std::string &flag,
                                     const std::string &line) {
    std::vector<std::string> result;

    // Cria regex que captura tudo dentro dos parênteses do comando
    std::string regex_str = flag + R"(\s*\(([^)]*)\))";
    std::regex pattern(regex_str);
    std::smatch match;

    if (std::regex_search(line, match, pattern) && match.size() > 1) {
        std::string inside = match[1].str(); // conteúdo entre os parênteses
        std::istringstream iss(inside);
        std::string token;
        while (iss >> token) {
            result.push_back(token);
        }
    }

    return result;
}

std::string cmakelists_str() {
    if (!stdfs::exists("CMakeLists.txt"))
        return "";
    return core::fs::read_to_string("CMakeLists.txt");
}
stdfs::path cmakelists_path() {
    if (!stdfs::exists("CMakeLists.txt"))
        return "";
    return "CMakeLists.txt";
}

} // namespace core::cmake