#pragma once

#include <CLI/CLI.hpp>

class subcommand {
   protected:
    CLI::App* cmd;

   public:
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