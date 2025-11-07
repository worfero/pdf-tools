#include "parser.hpp"
#include "xref-table.hpp"

uint64_t find_xref_addr(std::ifstream& file){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    size_t read_size = std::min<size_t>(LOOKBACK, file_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(read_size); // declare char buffer to read the section
    file.seekg((size_t)file_size - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
    file.read(buffer.data(), read_size); // effectively read the last 4kb of the file

    for (ssize_t i = read_size - 9; i >= 0; --i) { // "startxref" is 9 characters, so the start index must be moved 9 bytes backwards
        if (std::memcmp(buffer.data() + i, "startxref", 9) == 0) { // compares the current 9 bytes of the buffer with "startxref"
            std::string num; // gets the xref-table address as a string
            for (size_t j = i + 9; j < read_size; ++j) {
                if (isdigit(buffer[j])) num.push_back(buffer[j]); // gets the 10 (possible) digits of the address
                else if (!num.empty()) break;
            }
            if (!num.empty()){
                return std::stoull(num); // returns the address as a uint64_t variable
            }
        }
    }
    return XREF_ADDR_ERROR;
}

void parsePDF(std::string pdf){
    std::ifstream file(pdf, std::ios::binary);
    if (!file) {
        std::cerr << "Error: cannot open file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "File opened successfully!" << "\n\n";
    uint64_t xref_addr = find_xref_addr(file);
    if(xref_addr == XREF_ADDR_ERROR){
        std::cerr << "Error: xref-table address not found.\n";
        exit(EXIT_FAILURE);
    }
    std::cout << xref_addr << std::endl;
}