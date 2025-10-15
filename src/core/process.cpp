#include <filesystem>
#include <iostream>
#include <string>
#include <tiny-process-library/process.hpp>
namespace core::process {
int run(const std::string& command, const std::filesystem::path& path) {
    TinyProcessLib::Process process(
        command, path,
        [](const char* bytes, size_t n) { std::cout << bytes << std::endl; });
    return process.get_exit_status();
}
}  // namespace core