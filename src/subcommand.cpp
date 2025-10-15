#include "subcommand.h"

#include "CLI/CLI.hpp"
#include "core/core.h"
#include "string"

void build_subcommand::add(CLI::App& parent_app) {
    cmd = parent_app.add_subcommand("build", "Builds your application");
    release = false;
    cmd->add_flag("-r,!-d", release, "Builds in release profile");
}

void build_subcommand::run() {
    if (!cmd->parsed()) return;

    core::fs::ensure_dir("build");
    core::fs::require_file("CMakeLists.txt");

    std::string build_flag = "-DCMAKE_BUILD_TYPE=Debug";
    if (release) {
        "-DCMAKE_BUILD_TYPE=Release";
    }

    core::process::run("cmake -S . -B build " + build_flag, "");

    core::process::run("cmake --build build", "");
}