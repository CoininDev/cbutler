#pragma once

#include <CLI/CLI.hpp>
#include <core/core.h>
#include <string>
#include <vector>

// const std::filesystem::path SCAFFOLDS_DIR = "/usr/share/cbutler/scaffolds/";
const std::filesystem::path SCAFFOLDS_DIR =
    "/home/xab/Documentos/CLion/cbutler/scaffolds/";

class subcommand {
  protected:
    CLI::App *cmd;

  public:
    subcommand(CLI::App *cmd) : cmd(cmd) {
        cmd->callback([this]() { this->run(); });
    }
    virtual ~subcommand() = default;
    virtual void run() = 0;
};

class build_subcommand : public subcommand {
  private:
    bool release = false;

  public:
    build_subcommand(CLI::App &parent_app);
    void run() override;
};

class clean_subcommand : public subcommand {
  public:
    clean_subcommand(CLI::App &parent_app);
    void run() override;
};

class new_subcommand : public subcommand {
  private:
    std::filesystem::path _scaffold_path;
    std::string _project_name;
    bool _create_folder = true;

  public:
    new_subcommand(CLI::App &parent_app);
    void run() override;
};

class run_subcommand : public subcommand {
  private:
    bool release = false;
    std::string _project_name = "";

  public:
    run_subcommand(CLI::App &parent_app);
    void run() override;
};

class mod_new_subcommand : public subcommand {
  private:
    std::filesystem::path _scaffold_path;
    std::string _mod_name;

  public:
    mod_new_subcommand(CLI::App &parent_app)
        : subcommand(parent_app.add_subcommand(
              "new", "Creates new module in project")) {
        cmd->add_option("-n,--name", _mod_name, "Defines module name")
            ->required();
        _scaffold_path = "mod";
    }

    ~mod_new_subcommand() override = default;

    void run() override {
        core::fs::ensure_dir("src/");
        core::fs::ensure_dir("src/" + _mod_name);
        core::scaffold::scaffold scaffold(SCAFFOLDS_DIR / "mod",
                                          "src/" + _mod_name);
        scaffold.set_variable("MOD_NAME", _mod_name);
        scaffold.clone();

        core::butlerxml::append_line_in_section(
            "modules", "add_subdirectory(src/" + _mod_name + ")",
            core::cmake::cmakelists_path());
    }
};

class mod_subcommand : public subcommand {
  private:
    std::vector<std::string> submodule_names{};
    mod_new_subcommand new_cmd;

  public:
    mod_subcommand(CLI::App &parent_app);
    ~mod_subcommand() override = default;
    void run() override;
};