#pragma once

#include <filesystem>

namespace core::fs {

void ensure_dir(const std::filesystem::path& dir);
void require_file(const std::filesystem::path& file);

}  // namespace core::fs
