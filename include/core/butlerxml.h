#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/scaffolding.h"
// const std::string _left = "%<";
// const std::string right_ = ">%";
// const std::string end_mark = "/";
// const std::string comment = "#";

namespace stdfs = std::filesystem;

namespace core::butlerxml {

std::vector<std::string> get_section(const std::string& section_name,
                                     const std::string& raw);

void append_line_in_section(const std::string& section_name,
                            const std::string& line, const stdfs::path& path);

void overwrite_section(const std::string& section_name,
                       const std::string& new_content, const stdfs::path& path);

class ConfigParser {
   protected:
    std::unordered_map<std::string, std::string> _variables;
    std::unordered_map<std::string, std::string> _codeblocks;
    std::string _replace_vars(const std::string& content) {
        auto _left = "<__";
        auto right_ = "__>";
        std::string result = content;

        for (const auto& [vkey, vval] : _variables) {
            auto placeholder = _left + vkey + right_;
            size_t pos = 0;

            while ((pos = result.find(placeholder, pos)) != std::string::npos) {
                result.replace(pos, placeholder.length(), vval);
                pos += vval.length();
            }
        }
        return result;
    }

   public:
    virtual ~ConfigParser() = default;
    virtual void gen_codeblocks();
    std::string parse(const std::string& key) {
        auto val = _codeblocks[key];
        return _replace_vars(val);
    }
    void set_variable(const std::string& key, const std::string& value) {
        _variables[key] = value;
    }
    void set_variables(
        const std::unordered_map<std::string, std::string>& vars) {
        _variables = vars;
    }
    void clean_variables() { _variables = {}; }
};
}  // namespace core::butlerxml
