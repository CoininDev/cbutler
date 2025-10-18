#pragma once

#include <core/fs.h>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::config {

struct ConfigData {
    std::string project_name;
    std::vector<std::string> compilers;
    std::string compile_flags;
    std::vector<std::string> sets;
    std::vector<std::string> modules;
    std::vector<std::string> dependencies;
};

class IConfigStrategy {
   public:
    virtual ~IConfigStrategy() = default;

    virtual void read(ConfigData& data) = 0;
    virtual void write(const ConfigData& data) = 0;

    virtual void build(const stdfs::path& build_path, bool release,
                       const ConfigData& data) = 0;
    virtual void run(const stdfs::path& build_path, const ConfigData& data) = 0;
};

class CMakeConfigStrategy : public IConfigStrategy {
    void read(ConfigData& data) override;
    void write(const ConfigData& data) override;
    void build(const stdfs::path& build_path, bool release,
               const ConfigData& data) override;
    void run(const stdfs::path& build_path, const ConfigData& data) override;
};

class Config {
   public:
    void write_now();
    Config();
    ~Config();
    ConfigData data;
    std::unique_ptr<IConfigStrategy> strategy;
};

}  // namespace core::config
