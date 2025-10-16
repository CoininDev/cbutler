#include "core/scaffolding.h"

#include <regex>

#include "core/str.h"

using namespace core::scaffolding;

//
// ===== ProjectTemplate =====
//

void ProjectTemplate::_copy_files() {
    for (const auto& entry : stdfs::recursive_directory_iterator(_src_path)) {
        const auto& current_src = entry.path();
        auto relative = stdfs::relative(current_src, _src_path);
        auto current_dst = _dst_path / relative;
        if (entry.is_directory()) {
            stdfs::create_directories(current_dst);
        } else {
            stdfs::copy_file(current_src, current_dst);
        }
    }
}

std::string ProjectTemplate::_replace_vars(const std::string& content) {
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

void ProjectTemplate::_interpret_names() {
    std::vector<std::pair<stdfs::path, stdfs::path>> renomeacoes;

    for (const auto& entry : stdfs::recursive_directory_iterator(_dst_path)) {
        auto old_path = entry.path();
        std::string old_name = old_path.filename().string();

        std::string new_name = _replace_vars(old_name);
        if (new_name != old_name) {
            auto new_path = old_path.parent_path() / new_name;
            renomeacoes.emplace_back(old_path, new_path);
        }
    }

    // Execute os renomeamentos
    for (const auto& [old_path, new_path] : renomeacoes) {
        stdfs::rename(old_path, new_path);
    }
}

void ProjectTemplate::_interpret_content() {
    for (const auto& entry : stdfs::recursive_directory_iterator(_dst_path)) {
        auto content = core::fs::read_to_string(entry.path());
        auto new_content = _replace_vars(content);
        core::fs::overwrite(entry.path(), new_content);
    }
}

void ProjectTemplate::setVariable(std::string key, std::string val) {
    _variables[key] = val;
}

void ProjectTemplate::setVariables(
    std::unordered_map<std::string, std::string> vars) {
    _variables = vars;
}

void ProjectTemplate::clone() {
    _copy_files();
    _interpret_names();
    _interpret_content();
}

//
// ===== CodeBlockLibrary =====
//

void CodeBlockLibrary::setVariable(std::string key, std::string val) {
    _variables[key] = val;
}

void CodeBlockLibrary::setVariables(
    std::unordered_map<std::string, std::string> vars) {
    _variables = vars;
}

std::string CodeBlockLibrary::parse(std::string text) {
    auto block = code_block[text];
    for (auto [k, v] : _variables) {
        core::str::replace_all(block, k, v);
    }
    return block;
}

//
// ===== PseudoXmlParser =====
//

// helper functions
std::string xml_wrap(std::string s) { return _left + s + right_; }
std::string xml_wrap_end(std::string s) {
    return _left + end_mark + s + right_;
}
std::regex xml_wrap_single_regex(const std::string& tag_name) {
    // Regex resultará em algo como:
    // ^#%<dependency\b[^>]*\/>%$
    return std::regex("^" + comment + _left + tag_name + R"(\b[^>]*\/>)" +
                      right_ + "$");
}
std::pair<int, int> xml_line_bounds(std::string section_name,
                                    std::string raw_text) {
    auto start_exp = xml_wrap(section_name);
    auto end_exp = xml_wrap_end(section_name);
    int start_line = 0;
    int end_line = 0;
    std::tie(std::ignore, start_line) =
        core::str::find_line_with(raw_text, start_exp);
    std::tie(std::ignore, end_line) =
        core::str::find_line_with(raw_text, end_exp);
    return {start_line, end_line};
}
bool is_section_valid(int start_line, int end_line) {
    return start_line >= 0 || end_line >= 0 || end_line > start_line;
}

std::string PseudoXmlParser::find_section(std::string section_name) {
    auto raw_text = core::fs::read_to_string(current_file);
    auto [start_line, end_line] = xml_line_bounds(section_name, raw_text);

    if (!is_section_valid(start_line, end_line)) return "";

    auto lines = core::str::split_lines(raw_text);

    // don't include pseudoxml comments
    std::vector<std::string> section_lines(lines.begin() + start_line + 1,
                                           lines.begin() + end_line);

    return core::str::join_lines(section_lines);
}

void PseudoXmlParser::append_section(std::string section_name,
                                     std::string append) {
    auto raw = find_section(section_name);
    auto lines = core::str::split_lines(raw);
    lines.push_back(append);
    overwrite_section(section_name, core::str::join_lines(lines));
}

void PseudoXmlParser::overwrite_section(std::string section_name,
                                        std::string new_section) {
    auto raw_text = core::fs::read_to_string(current_file);
    auto [start_line, end_line] = xml_line_bounds(section_name, raw_text);
    auto start_exp = xml_wrap(section_name);
    auto end_exp = xml_wrap_end(section_name);

    if (!is_section_valid(start_line, end_line)) {
        // Remove old invalid comment, in case they exist
        core::str::replace_all(raw_text, comment + start_exp, "");
        core::str::replace_all(raw_text, comment + end_exp, "");

        // Add new section at the end
        std::string file_new = raw_text + "\n" + comment + start_exp + "\n" +
                               new_section + "\n" + comment + end_exp + "\n";
        core::fs::overwrite(current_file, file_new);
        return;
    }

    auto file_lines = core::str::split_lines(raw_text);
    std::vector<std::string> file_clean;

    for (int i = 0; i <= start_line; ++i) {
        file_clean.push_back(file_lines[i]);
    }

    auto new_lines = core::str::split_lines(new_section);
    file_clean.insert(file_clean.end(), new_lines.begin(), new_lines.end());

    for (int i = end_line; i < (int)file_lines.size(); ++i) {
        file_clean.push_back(file_lines[i]);
    }

    std::string joined = core::str::join_lines(file_clean);
    core::fs::overwrite(current_file, joined);
}
std::vector<std::unordered_map<std::string, std::string>>
PseudoXmlParser::find_single_tags(std::string tag_name) {
    auto regex_exp = xml_wrap_single_regex(tag_name);
    auto lines = core::fs::read_lines(current_file);
    std::vector<std::string> matches;
    for (const auto& s : lines) {
        if (std::regex_match(s, regex_exp)) {
            matches.push_back(s);
        }
    }

    std::vector<std::unordered_map<std::string, std::string>> result;
    for (const auto& tag : matches) {
        result.push_back(single_tag(tag));
    }
    return result;
}

std::unordered_map<std::string, std::string> PseudoXmlParser::single_tag(
    std::string line) {
    std::unordered_map<std::string, std::string> tag;

    // get tag name
    std::regex tag_regex(R"(#%<(\w+))");
    std::smatch tag_match;

    if (std::regex_search(line, tag_match, tag_regex)) {
        tag["_TAG"] = tag_match[1];
    }

    // get attributes
    std::regex attr_regex(R"DELIM((\w+)\s*=\s*"([^"]*)")DELIM");
    auto words_begin =
        std::sregex_iterator(line.begin(), line.end(), attr_regex);
    auto words_end = std::sregex_iterator();

    for (auto i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        tag[match[1]] = match[2];
    }
    return tag;
}