#pragma once

#include <core/fs.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace stdfs = std::filesystem;

const std::string _left = "%<";
const std::string right_ = ">%";
const std::string end_mark = "/";
const std::string comment = "#";

namespace core::scaffolding {
class ProjectTemplate {
   public:
    void setVariable(std::string key, std::string val);
    void setVariables(std::unordered_map<std::string, std::string> vars);
    void clone();
    ProjectTemplate(std::string src, std::string dst)
        : _src_path(src), _dst_path(dst) {}

   private:
    stdfs::path _src_path;
    stdfs::path _dst_path;
    std::unordered_map<std::string, std::string> _variables;
    std::string _replace_vars(const std::string& content);
    void _copy_files();
    void _interpret_names();
    void _interpret_content();
};

class CodeBlockLibrary {
   public:
    void setVariable(std::string key, std::string val);
    void setVariables(std::unordered_map<std::string, std::string> vars);
    std::string parse(std::string text);
    std::unordered_map<std::string, std::string> code_block;

   private:
    std::unordered_map<std::string, std::string> _variables;
};

class PseudoXmlParser {
   public:
    std::string wrap(std::string s);
    std::string wrap_end(std::string s);
    std::string wrap_single(std::unordered_map<std::string, std::string> ss);
    std::string find_section(std::string);
    std::vector<std::unordered_map<std::string, std::string>> find_single_tags(
        std::string tag_name);
    std::unordered_map<std::string, std::string> single_tag(std::string line);
    void append_section(std::string section_name, std::string append);
    void overwrite_single_tag(const std::string& tagname, size_t index,
                              std::unordered_map<std::string, std::string> map,
                              stdfs::path path);
    void overwrite_section(std::string section_name, std::string new_section);
    stdfs::path current_file;
};

}  // namespace core::scaffolding
