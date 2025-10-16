#include "core/config.h"

#include <filesystem>
#include <regex>

#include "core/fs.h"
#include "core/scaffolding.h"
#include "core/str.h"

namespace stdfs = std::filesystem;

using namespace core::config;

//
// ===== CMakeConfigStrategy =====
//
std::vector<std::string> inside_flag(const std::string& flag,
                                     const std::string& line) {
    std::vector<std::string> result;

    // Cria regex que captura tudo dentro dos parênteses do comando
    std::string regex_str = flag + R"(\s*\(([^)]*)\))";
    std::regex pattern(regex_str);
    std::smatch match;

    if (std::regex_search(line, match, pattern) && match.size() > 1) {
        std::string inside = match[1].str();  // conteúdo entre os parênteses
        std::istringstream iss(inside);
        std::string token;
        while (iss >> token) {
            result.push_back(token);
        }
    }

    return result;
}

std::vector<std::string> parse_sets(std::string raw) {
    std::vector<std::string> result;
    auto lines = core::str::split_lines(raw);
    for (const std::string& line : lines) {
        auto val = inside_flag("set", line);
        result.push_back(val[0] + "=" + val[1]);
    }
    return result;
}

std::vector<std::string> parse_modules(std::string raw) {
    std::vector<std::string> result;
    auto lines = core::str::split_lines(raw);
    for (const std::string& line : lines) {
        auto val = inside_flag("set", line)[0];
        std::regex prefix(R"(src/)");
        auto new_val = std::regex_replace(val, prefix, "");
    }
    return result;
}
std::vector<std::string> parse_dependencies(std::string raw) {
    std::vector<std::string> result;
    core::scaffolding::PseudoXmlParser xml;
    auto lines = core::str::split_lines(raw);
    for (const std::string& line : lines) {
        auto tag = xml.single_tag(line);
        if (tag["_TAG"] != "dependency") continue;
        auto id = tag["id"];
        auto src = tag["src"];
        result.push_back(id + "=" + src);
    }
    return result;
}

void CMakeConfigStrategy::read(ConfigData& data) {
    auto filename = "CMakeLists.txt";
    // TODO: Ler dados de configuração de um arquivo CMake (ex: CMakeLists.txt)
    std::string raw;
    if (stdfs::exists(filename)) raw = core::fs::read_to_string(filename);
    core::scaffolding::PseudoXmlParser xml;

    std::string projectline;
    std::tie(projectline, std::ignore) =
        core::str::find_line_with(raw, "project(");
    data.project_name = inside_flag("project", projectline)[0];

    auto sets_raw = xml.find_section("sets");
    auto modules_raw = xml.find_section("modules");
    auto dependencies_raw = xml.find_section("dependencies");

    data.sets = parse_sets(sets_raw);
    data.modules = parse_modules(modules_raw);
    data.dependencies = parse_dependencies(dependencies_raw);
}

void CMakeConfigStrategy::write(const ConfigData& data) {
    // TODO: Escrever dados de configuração em um arquivo CMake (ex:
    // CMakeLists.txt)
    core::scaffolding::CodeBlockLibrary cbl;
    cbl.code_block["project"] = "project(<__PROJECT_NAME__>)";
    cbl.code_block["set"] = "set(<__KEY__> <__VALUE__>)";
    cbl.code_block["module"] = "add_subdirectory(src/<__MOD_NAME__>)";
    cbl.code_block["enable_fetch_content"] = "include(FetchContent)";
    cbl.code_block["init"] = R"(
        cmake_minimum_required(VERSION 3.24.0)
        project(<__PROJECT_NAME__>)
    )";
    cbl.code_block["dependency"] = R"(
        #%<dependency id="<__ID__>" src="<__SRC__>" />%
        FetchContent_Declare(
            <__ID__>
            URL <__SRC__>
            SOURCE_DIR <__DIR__>
        )
        FetchContent_MakeAvailable(<__ID__>)
        if(EXISTS "<__DIR__>/CMakeLists.txt")
            add_subdirectory(<__DIR__>)
        endif()
    )";

    auto filename = "CMakeLists.txt";
    ConfigData disk_data;
    read(disk_data);
    std::string raw;
    if (stdfs::exists(filename)) raw = core::fs::read_to_string(filename);

    if (disk_data.project_name != data.project_name) {
        int line;
        std::tie(std::ignore, line) =
            core::str::find_line_with(raw, "project(");
    }

    if (disk_data.sets != data.sets) {
    }

    if (disk_data.modules != data.modules) {
    }

    if (disk_data.dependencies != data.dependencies) {
    }
}

//
// ===== Config =====
//

Config::Config() = default;
Config::~Config() = default;

void Config::write_now() {
    if (strategy) {
        strategy->write(data);
    }
}
