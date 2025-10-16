#include <core/butlerxml.h>
#include <core/fs.h>
#include <core/str.h>

#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::butlerxml {

std::vector<std::string> get_section(const std::string &section_name,
                                     const stdfs::path &path) {
    auto start_exp = _left + section_name + right_;
    auto end_exp = _left + end_mark + section_name + right_;
    int start_line = 0;
    int end_line = 0;
    std::tie(std::ignore, start_line) =
        core::fs::find_line_with(path, start_exp);
    std::tie(std::ignore, end_line) = core::fs::find_line_with(path, end_exp);

    if (start_line < 0 || end_line < 0 || end_line <= start_line)
        return {}; // not found or invalid

    auto lines = core::fs::read_lines(path);

    // don't include pseudoxml comments
    std::vector<std::string> section_lines(lines.begin() + start_line + 1,
                                           lines.begin() + end_line);

    return section_lines;
}

void append_line_in_section(const std::string &section_name,
                            const std::string &line, const stdfs::path &path) {
    auto section = get_section(section_name, path);
    section.push_back(line);
    overwrite_section(section_name, core::str::join_lines(section), path);
}

void overwrite_section(const std::string &section_name,
                       const std::string &new_content,
                       const stdfs::path &path) {
    auto start_exp = _left + section_name + right_;
    auto end_exp = _left + end_mark + section_name + right_;
    int start_line = 0;
    int end_line = 0;
    std::tie(std::ignore, start_line) =
        core::fs::find_line_with(path, start_exp);
    std::tie(std::ignore, end_line) = core::fs::find_line_with(path, end_exp);

    auto file_raw = core::fs::read_to_string(path);

    // invalid or absent
    if (start_line < 0 || end_line < 0 || end_line <= start_line) {
        // Remove old invalid comment, in case they exist
        core::str::replace_all(file_raw, comment + start_exp, "");
        core::str::replace_all(file_raw, comment + end_exp, "");

        // Add new section at the end
        std::string file_new = file_raw + "\n" + comment + start_exp + "\n" +
                               new_content + "\n" + comment + end_exp + "\n";
        core::fs::overwrite(path, file_new);
        return;
    }
    // valid
    auto file_lines = core::fs::read_lines(path);
    std::vector<std::string> file_clean;

    for (int i = 0; i <= start_line; ++i) {
        file_clean.push_back(file_lines[i]);
    }

    auto new_lines = core::str::split_lines(new_content);
    file_clean.insert(file_clean.end(), new_lines.begin(), new_lines.end());

    for (int i = end_line; i < (int)file_lines.size(); ++i) {
        file_clean.push_back(file_lines[i]);
    }

    std::string joined = core::str::join_lines(file_clean);
    core::fs::overwrite(path, joined);
}

} // namespace core::butlerxml