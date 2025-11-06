#include "parser.hpp"

int parsePDF(std::string pdf){
    std::ifstream file(pdf);
    if (!file) {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }

    std::cout << "File opened successfully!" << "\n";
    return 0;
}