#include <core/core.h>
#include <subcommand.h>

#include <CLI/CLI.hpp>
#include <iostream>
#include <tiny-process-library/process.hpp>

int main(int argc, char **argv) {
    CLI::App app{R"(
        Cbutler is a CLI tool for managing projects with C/C++.
    )"};

    build_subcommand build_cmd(app);
    clean_subcommand clean_cmd(app);
    new_subcommand new_cmd(app);
    run_subcommand run_cmd(app);
    mod_subcommand mod_cmd(app);

    app.parse(argc, argv);

    return 0;
}