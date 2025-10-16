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

  private:
    stdfs::path _src_path;
    stdfs::path _dst_path;
    std::unordered_map<std::string, std::string> _variables;
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
    std::string find_section(std::string);
    void append_section(std::string section_name, std::string append);
    void overwrite_section(std::string section_name, std::string new_section);
    stdfs::path current_file;
};

} // namespace core::scaffolding
