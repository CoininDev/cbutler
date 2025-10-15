#pragma once
#include <filesystem>
#include <unordered_map>

namespace stdfs = std::filesystem;

namespace core::scaffold {
class scaffold {
   private:
    stdfs::path _src_path, _dst_path;
    std::unordered_map<std::string, std::string> _variables;

    std::string _replace_vars(const std::string& content);
    void _copy_files();
    void _interpret_names();
    void _interpret_content();

   public:
    scaffold(stdfs::path src_path, stdfs::path dst_path)
        : _src_path(src_path), _dst_path(dst_path) {}

    void clone();

    void set_variable(const std::string& key, const std::string& value) {
        _variables[key] = value;
    }

    void set_variables(
        const std::unordered_map<std::string, std::string>& vars) {
        _variables = vars;
    }
};
}  // namespace core::scaffold