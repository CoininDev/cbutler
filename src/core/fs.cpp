#include <core/fs.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace core::fs {

void ensure_dir(const std::filesystem::path &dir) {
    if (std::filesystem::exists(dir))
        return;

    std::error_code ec;
    if (!std::filesystem::create_directory(dir, ec)) {
        std::cerr << "Ocorreu um erro ao criar diretório " << dir << ": " << ec
                  << std::endl;
    }
}

void remove_dir(const std::filesystem::path &dir) {
    if (!std::filesystem::exists(dir))
        return;

    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    if (ec) {
        std::cerr << "Não foi possível remover: " << dir << ": " << ec
                  << std::endl;
    }
}

void require_file(const std::filesystem::path &file) {
    if (!std::filesystem::exists(file)) {
        std::cerr << file << " não encontrado. (require_file)" << std::endl;
    }
}

std::string read_to_string(const std::filesystem::path &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Erro: Não foi possível abrir " +
                                 path.string());

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::vector<std::string> read_lines(const std::filesystem::path &path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Erro: Não foi possível abrir " +
                                 path.string());

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        lines.push_back(line);
    }

    return lines;
}

std::string join_lines(const std::vector<std::string> &lines) {
    std::ostringstream out;
    for (size_t i = 0; i < lines.size(); ++i) {
        out << lines[i];
        if (i + 1 < lines.size()) // evita \n extra no final
            out << '\n';
    }
    return out.str();
}

void overwrite(const std::filesystem::path &path, const std::string &content) {
    std::ofstream file(path);
    file << content;
    file.close();
}

std::tuple<std::string, int> find_line_with(const std::filesystem::path &path,
                                            const std::string &exp) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << path << std::endl;
        return std::tuple<std::string, int>("", -1);
    }

    std::string line;
    int line_number = 1;

    while (std::getline(file, line)) {
        if (line.find(exp) != std::string::npos) {
            return std::tuple<std::string, int>(line, line_number);
        }
        line_number++;
    }

    return std::tuple<std::string, int>("", -1);
}

} // namespace core::fs