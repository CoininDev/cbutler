#include "core/fs.h"

#include "filesystem"
#include "iostream"
#include "string"

namespace core::fs {

void ensure_dir(const std::filesystem::path& dir) {
    if (std::filesystem::exists(dir)) return;

    std::error_code ec;
    if (!std::filesystem::create_directory(dir, ec)) {
        std::cerr << "Ocorreu um erro ao criar diretório " << dir << ": " << ec
                  << std::endl;
    }
}

void require_file(const std::filesystem::path& file) {
    if (!std::filesystem::exists(file)) {
        std::cerr << file << " não encontrado. (require_file)" << std::endl;
    }
}

}  // namespace core::fs