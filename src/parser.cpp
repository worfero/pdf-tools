#include "parser.hpp"

uint64_t find_xref_addr(std::ifstream &file){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    size_t read_size = std::min<size_t>(LOOKBACK, file_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(read_size); // declare char buffer to read the section
    file.seekg(static_cast<size_t>(file_size) - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
    file.read(buffer.data(), read_size); // effectively read the last 4kb of the file

    for(ssize_t i = read_size - 9; i >= 0; --i){ // "startxref" is 9 characters, so the start index must be moved 9 bytes backwards
        if(std::memcmp(buffer.data() + i, "startxref", 9) == 0){ // compares the current 9 bytes of the buffer with "startxref"
            std::string num; // gets the xref-table address as a string
            for(size_t j = i + 9; j < read_size; ++j){
                if(isdigit(buffer[j])) num.push_back(buffer[j]); // gets the 10 (possible) digits of the address
                else if(!num.empty()) break;
            }
            if(!num.empty()){
                return std::stoull(num); // returns the address as a uint64_t variable
            }
        }
    }
    return XREF_OFFSET_ERROR;
}

std::vector<X_Ref_Entry> read_xref_table(std::ifstream &file, uint64_t xref_offset){
    file.seekg(xref_offset); // move the pointer to the xref offset
    std::string token;
    file >> token;
    if(token != "xref") throw std::runtime_error("Error: xref keyword not found"); // make sure the xref table is there

    int first_entry, count;
    file >> first_entry >> count; // get the first entry index and the number of entries

    std::vector<X_Ref_Entry> xref_table(count); // allocate a vector with the number of entries size

    for(int i = 0; i < count; i++){
        std::string offset_str, gen_str, flag;
        file >> offset_str >> gen_str >> flag;
        xref_table[i] = {
            static_cast<uint64_t>(std::stoull(offset_str)),
            static_cast<uint16_t>(std::stoi(gen_str)),
            flag == "n"
        };
    }

    return xref_table;
}

void parsePDF(std::string pdf){
    std::ifstream file(pdf, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: cannot open file");
    }

    std::cout << "File opened successfully." << "\n\n";

    uint64_t xref_offset = find_xref_addr(file);
    if(xref_offset == XREF_OFFSET_ERROR){
        throw std::runtime_error("Error: xref-table address not found.");
    }
    std::cout << "xref table offset: " << xref_offset << "\n\n";

    std::vector<X_Ref_Entry> xref_table = read_xref_table(file, xref_offset);
    std::cout << "[" << "\n";
    for(size_t i = 0; i < xref_table.size(); i++){
        std::cout << " " << xref_table[i].offset << " " << xref_table[i].generation << " " << xref_table[i].inUse << "\n";
    }
    std::cout << "]" << std::endl;
}