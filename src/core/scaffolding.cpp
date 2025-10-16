#include "core/scaffolding.h"
#include "core/str.h"

using namespace core::scaffolding;

//
// ===== ProjectTemplate =====
//

void ProjectTemplate::setVariable(std::string key, std::string val) {
    _variables[key] = val;
}

void ProjectTemplate::setVariables(
    std::unordered_map<std::string, std::string> vars) {
    _variables = vars;
}

void ProjectTemplate::clone() {
    // TODO: Implementar cópia do template de projeto (_src_path -> _dst_path)
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
std::string xml_wrap_single(std::string s) {
    return _left + s + end_mark + right_;
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

    if (!is_section_valid(start_line, end_line))
        return "";

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
