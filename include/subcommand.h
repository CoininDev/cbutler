#pragma once

#include <CLI/CLI.hpp>

class subcommand {
   protected:
    CLI::App* cmd;

   public:
    subcommand(CLI::App* cmd) : cmd(cmd) {
        cmd->callback([this]() { this->run(); });
    }
    virtual ~subcommand() = default;
    virtual void run() = 0;
};

class build_subcommand : public subcommand {
   private:
    bool release = false;

   public:
    build_subcommand(CLI::App& parent_app);
    void run() override;
};

class clean_subcommand : public subcommand {
   public:
    clean_subcommand(CLI::App& parent_app);
    void run() override;
};

class new_subcommand : public subcommand {
   private:
    std::filesystem::path _scaffold_path;
    std::string _project_name;

   public:
    new_subcommand(CLI::App& parent_app);
    void run() override;
};
