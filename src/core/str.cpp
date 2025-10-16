#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace core::str {

std::vector<std::string> split_lines(const std::string& raw) {
    std::vector<std::string> lines;
    std::istringstream stream(raw);
    std::string line;

    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> split_char(const char c, std::string raw) {
    size_t pos = raw.find(c);
    if (pos != std::string::npos) {
        std::string chave = raw.substr(0, pos);
        std::string valor = raw.substr(pos + 1);
        return {chave, valor};
    }
}

std::string join_lines(const std::vector<std::string>& lines) {
    std::ostringstream stream;

    for (size_t i = 0; i < lines.size(); ++i) {
        stream << lines[i];
        if (i != lines.size() - 1) {
            stream << '\n';
        }
    }

    return stream.str();
}

std::tuple<std::string, int> find_line_with(const std::string& raw,
                                            const std::string& exp) {
    std::istringstream stream(raw);
    std::string line;
    int line_number = 0;

    while (std::getline(stream, line)) {
        if (line.find(exp) != std::string::npos) {
            return std::make_tuple(line, line_number);
        }
        ++line_number;
    }

    return std::make_tuple("", -1);
}

void replace_all(std::string& str, const std::string& from,
                 const std::string& to) {
    if (from.empty()) return;  // evita loop infinito
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();  // avança para não substituir dentro do que
                             // acabou de colocar
    }
}

}  // namespace core::str