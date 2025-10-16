#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::butlerxml {

std::vector<std::string> get_section(const std::string &section_name,
                                     const std::string &raw);

void append_line_in_section(const std::string &section_name,
                            const std::string &line, const stdfs::path &path);

void overwrite_section(const std::string &section_name,
                       const std::string &new_content, const stdfs::path &path);

} // namespace core::butlerxml