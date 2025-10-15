#include <core/fs.h>
#include <core/scaffold.h>

#include <string>
#include <vector>

namespace stdfs = std::filesystem;

namespace core::scaffold {

void scaffold::_copy_files() {
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

std::string scaffold::_replace_vars(const std::string& content) {
    auto _left = "<__";
    auto right_ = "__>";
    std::string result = "";

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

void scaffold::_interpret_names() {
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

void scaffold::_interpret_content() {
    for (const auto& entry : stdfs::recursive_directory_iterator(_dst_path)) {
        auto content = core::fs::read_to_string(entry.path());
        auto new_content = _replace_vars(content);
        core::fs::overwrite(entry.path(), new_content);
    }
}

void scaffold::clone() {
    _copy_files();
    _interpret_names();
    _interpret_content();
}

}  // namespace core::scaffold