#include "core/config.h"

#include <algorithm>
#include <filesystem>
#include <regex>
#include <type_traits>
#include <unordered_map>

#include "core/cmake.h"
#include "core/fs.h"
#include "core/process.h"
#include "core/scaffolding.h"
#include "core/str.h"
namespace stdfs = std::filesystem;

using namespace core::config;

//
// ===== CMakeConfigStrategy =====
//

std::vector<std::string> parse_sets(std::string raw) {
    std::vector<std::string> result;
    auto lines = core::str::split_lines(raw);
    for (const std::string& line : lines) {
        auto val = core::cmake::inside_flag("set", line);
        result.push_back(val[0] + "=" + val[1]);
    }
    return result;
}

std::vector<std::string> parse_modules(std::string raw) {
    std::vector<std::string> result;
    auto lines = core::str::split_lines(raw);
    for (const std::string& line : lines) {
        auto val = core::cmake::inside_flag("set", line)[0];
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

    std::string raw;
    if (stdfs::exists(filename)) raw = core::fs::read_to_string(filename);
    core::scaffolding::PseudoXmlParser xml;

    data.project_name = core::cmake::project_name(filename);

    std::unordered_map<std::string, std::string> compilers =
        xml.find_single_tags("compiler")[0];
    if (compilers.empty()) {
        // TODO: Make default cbutler compiler configurable
        compilers["c"] = "/usr/bin/clang";
        compilers["cxx"] = "/usr/bin/clang++";
    }
    std::vector<std::string> compiler_pairs;
    compiler_pairs.reserve(compilers.size());
    std::transform(
        compilers.begin(), compilers.end(), std::back_inserter(compiler_pairs),
        [](const auto& pair) { return pair.first + "=" + pair.second; });
    data.main_compiler = str::join_char(';', compiler_pairs);

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
    core::scaffolding::PseudoXmlParser xml;
    read(disk_data);
    std::string raw;
    if (stdfs::exists(filename)) raw = core::fs::read_to_string(filename);

    if (disk_data.project_name != data.project_name) {
        int line;
        std::tie(std::ignore, line) =
            core::str::find_line_with(raw, "project(");
        core::fs::replace_line(
            filename, line,
            core::cmake::wrap_flag("project", data.project_name));
    }

    if (disk_data.sets != data.sets) {
        std::vector<std::string> sets;
        for (auto set : data.sets) {
            auto cu = core::str::split_char('=', set);
            cbl.setVariable("KEY", cu[0]);
            cbl.setVariable("VALUE", cu[1]);
            sets.push_back(cbl.parse("set"));
        }

        xml.overwrite_section("sets", core::str::join_lines(sets));
    }

    if (disk_data.modules != data.modules) {
        std::vector<std::string> modules;
        for (auto mod : data.modules) {
            cbl.setVariable("MOD_NAME", mod);
            modules.push_back(cbl.parse("module"));
        }

        xml.overwrite_section("modules", core::str::join_lines(modules));
    }

    if (disk_data.dependencies != data.dependencies) {
        std::vector<std::string> deps;
        for (auto dep : data.dependencies) {
            auto cu = core::str::split_char('=', dep);
            cbl.setVariable("ID", cu[0]);
            cbl.setVariable("SRC", cu[1]);
            cbl.setVariable("DIR", "vendors/" + cu[0]);
            deps.push_back(cbl.parse("dependency"));
        }

        xml.overwrite_section("", core::str::join_lines(deps));
    }
}

void CMakeConfigStrategy::build(const stdfs::path& build_path, bool release,
                                const ConfigData& data) {
    core::fs::require_file("CMakeLists.txt");

    std::string build_flag = "-DCMAKE_BUILD_TYPE=Debug";
    if (release) {
        build_flag = "-DCMAKE_BUILD_TYPE=Release";
    }

    core::process::run(
        "cmake -S . -B+ " + build_path.string() + " " + build_flag, "");
    core::process::run("cmake --build" + build_path.string(), "");
}
void CMakeConfigStrategy::run(const stdfs::path& build_path,
                              const ConfigData& data) {
    core::process::run(build_path.string() + data.project_name, "");
}
//
// ===== Config =====
//

Config::Config() {
    if (stdfs::exists("CMakeLists.txt")) {
        strategy = std::make_unique<CMakeConfigStrategy>();
    }
};
Config::~Config() = default;

void Config::write_now() {
    if (strategy) {
        strategy->write(data);
    }
}
