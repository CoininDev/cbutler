#include "subcommand.h"

#include "CLI/CLI.hpp"
#include "core/core.h"
#include "string"

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

const std::filesystem::path SCAFFOLDS_DIR = "/usr/share/cbutler/scaffolds/";
new_subcommand::new_subcommand(CLI::App& parent_app)
    : subcommand(parent_app.add_subcommand(
          "new", "Creates a project based on scaffolds")) {
    cmd->add_option("-s,--scaffold", _scaffold_path, "Specify the scaffold")
        ->default_val("blank");
    cmd->add_option("-n,--name", _project_name, "Project name")->required();

    if (!_scaffold_path.string().contains("/")) {
        _scaffold_path = SCAFFOLDS_DIR / _scaffold_path;
    }
}

void new_subcommand::run() {
    core::scaffold::scaffold scaffold(_scaffold_path, ".");
    scaffold.set_variable("PROJECT_NAME", _project_name);
    scaffold.clone();
}