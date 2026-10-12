#include "calc.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

bool calculate(const std::string& expression) {
    try {
        const double result = evaluate(expression);
        std::cout << std::fixed << std::setprecision(2) << result << '\n';
        return true;
    } catch (const std::invalid_argument&) {
        std::cout << "invalid input!\n";
        return false;
    }
}

bool isExitCommand(const std::string& line) {
    const std::size_t first = line.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos) {
        return false;
    }
    const std::size_t last = line.find_last_not_of(" \t\r\n\v\f");
    const std::string command = line.substr(first, last - first + 1);
    return command == "exit" || command == "exit()" || command == "goodbye";
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc > 1) {
        bool success = true;
        for (int i = 1; i < argc; ++i) {
            if (!calculate(argv[i])) {
                success = false;
            }
        }
        return success ? 0 : 1;
    }

    std::string line;
    while (true) {
        std::cout << ">" << std::flush;
        if (!std::getline(std::cin, line) || isExitCommand(line)) {
            std::cout << "Goodbye!\n";
            break;
        }
        calculate(line);
    }
    return 0;
}
