#include <iostream>
#include <tiny-process-library/process.hpp>


int main() {
    TinyProcessLib::Process process(
        "mkdir teste",
        "",
        [](const char *bytes, size_t n) {
            std::cout << "output from stdout: " << bytes << std::endl;
    });
    auto exit_status = process.get_exit_status();
    std::cout << exit_status << std::endl;
    return 0;
}