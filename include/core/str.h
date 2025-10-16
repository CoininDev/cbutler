#pragma once
#include <string>
#include <tuple>
#include <vector>
namespace core::str {
std::vector<std::string> split_lines(const std::string& raw);
std::string join_lines(const std::vector<std::string>& raw);
std::tuple<std::string, int> find_line_with(const std::string& raw,
                                            const std::string& exp);
}  // namespace core::str
