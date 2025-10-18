#include "subcommand.h"

#include <filesystem>
#include <memory>

#include "CLI/CLI.hpp"
#include "core/core.h"
#include "string"

namespace stdfs = std::filesystem;

build_subcommand::build_subcommand(CLI::App& parent_app)
    : subcommand(
          parent_app.add_subcommand("build", "Builds your application")) {
    cmd->add_flag("-r,!-d", release, "Builds in release profile");
}

void build_subcommand::run() {
    if (!cmd->parsed()) return;

    core::fs::ensure_dir("build");
    core::fs::require_file("CMakeLists.txt");

    std::string build_flag = "-DCMAKE_BUILD_TYPE=Debug";
    if (release) {
        build_flag = "-DCMAKE_BUILD_TYPE=Release";
    }

    core::process::run("cmake -S . -B build " + build_flag, "");

    core::process::run("cmake --build build", "");
}

clean_subcommand::clean_subcommand(CLI::App& parent_app)
    : subcommand(parent_app.add_subcommand("clean", "Removes build folder")) {}

void clean_subcommand::run() { core::fs::remove_dir("build"); }

new_subcommand::new_subcommand(CLI::App& parent_app)
    : subcommand(parent_app.add_subcommand(
          "new", "Creates a project based on scaffolds")) {
    std::string current_dir_name = stdfs::current_path().filename().string();

    cmd->add_option("-s,--scaffold", _scaffold_path, "Specify the scaffold")
        ->default_val("blank");
    cmd->add_option("-n,--name", _project_name, "Project name");
}

void new_subcommand::run() {
    stdfs::path current_dir = stdfs::current_path();
    std::string current_dir_name = current_dir.filename().string();

    bool hasNameArg = !_project_name.empty();

    if (!_scaffold_path.string().contains("/")) {
        _scaffold_path = SCAFFOLDS_DIR / _scaffold_path;
    }

    if (!hasNameArg) {
        _project_name = current_dir_name;
        _create_folder = false;
    } else {
        _create_folder = true;
    }

    if (_create_folder) {
        core::fs::ensure_dir(_project_name);
    }

    core::scaffolding::ProjectTemplate templ(
        _scaffold_path,
        (_create_folder ? stdfs::path(_project_name) : stdfs::path(".")));
    templ.setVariable("PROJECT_NAME", _project_name);
    templ.clone();
}

run_subcommand::run_subcommand(CLI::App& parent_app)
    : subcommand(parent_app.add_subcommand("run", "Runs the subcommand")) {
    cmd->add_flag("-r,!-d", release, "Runs in release profile");
    _project_name = core::cmake::project_name("CMakeLists.txt");
}

void run_subcommand::run() {
    core::config::Config cfg;
    core::fs::ensure_dir(".tmp");
    cfg.strategy->build(".tmp", release, cfg.data);
    cfg.strategy->run(".tmp", cfg.data);
    core::fs::remove_dir(".tmp");
}

// mod_subcommand::mod_subcommand(CLI::App &parent_app)
//     : subcommand(parent_app.add_subcommand("mod", "Modules")), new_cmd(*cmd)
//     {}

// void mod_subcommand::run() {
//     // This subcommand doesn't need to do anything specific
//     // The actual work is handled by the sub-subcommands
// }
