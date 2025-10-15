#include <core/core.h>
#include <subcommand.h>

#include <CLI/CLI.hpp>
#include <iostream>
#include <tiny-process-library/process.hpp>

int main(int argc, char** argv) {
    CLI::App app{R"(
        Cbutler is a CLI tool for managing projects with C/C++.
    )"};

    build_subcommand build(app);
    clean_subcommand clean(app);
    new_subcommand new_cmd(app);

    app.parse(argc, argv);

    return 0;
}