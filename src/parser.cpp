#include "parser.hpp"

uint64_t Parser::find_xref_offset(std::ifstream &file){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_stream_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    
    size_t file_size = static_cast<size_t>(file_stream_size);

    size_t read_size = std::min<size_t>(LOOKBACK, file_stream_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(read_size); // declare char buffer to read the section
    
    file.seekg(file_size - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
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
    throw std::runtime_error("Error: xref-table address not found.");
}

void Parser::read_xref_table(std::ifstream &file, PDFDocument &pdf_doc, uint64_t xref_offset){
    file.seekg(xref_offset); // move the pointer to the xref offset
    std::string token;
    file >> token;
    if(token != "xref") throw std::runtime_error("Error: xref keyword not found."); // make sure the xref table is there

    while(1){
        int first_entry, count;
        file >> token;
        if(!isdigit(token[0])){
            break;
        }

        first_entry = std::stoi(token);
        file >> count;

        X_Ref_Table xref_table;
        xref_table.first_entry = first_entry;
        xref_table.count = count;

        for(int i = 0; i < count; i++){
            std::string offset_str, gen_str, flag;
            file >> offset_str >> gen_str >> flag;
            X_Ref_Entry entry = {
                static_cast<uint64_t>(std::stoull(offset_str)),
                static_cast<uint16_t>(std::stoi(gen_str)),
                flag[0]
            };
            xref_table.add_entry(entry);
        }
        pdf_doc.get_xref_tables().add_table(xref_table);
    }
}

void Parser::parse_trailer(std::ifstream& file, PDFDocument &pdf_doc){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_stream_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    
    size_t file_size = static_cast<size_t>(file_stream_size);

    size_t read_size = std::min<size_t>(LOOKBACK, file_stream_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(file_stream_size); // declare char buffer to read the section

    file.seekg(file_size - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
    file.read(buffer.data(), read_size); // effectively read the last 4kb of the file

    // trailer offsets relative to the ending 4kb of the file
    size_t trailer_offset = 0;
    size_t dict_offset = 0;
    size_t dict_end = 0;

    for(size_t i = read_size - 7; i >= 0; --i){ // "trailer" is 7 characters, so the start index must be moved 9 bytes backwards
        if(std::memcmp(buffer.data() + i, "trailer", 7) == 0){ // compares the last 7 bytes of the buffer with "trailer"
            trailer_offset = i;
            break;
        }
    }
    if(trailer_offset == 0) throw std::runtime_error("Error: trailer not found.");

    for(size_t i = trailer_offset + 7; i <= file_size; i++){ // check for trailer dict opening brackets
        if(std::memcmp(buffer.data() + i, "<<", 2) == 0){
            dict_offset = i + 2;
        }
        if(dict_offset != 0){
            if(std::memcmp(buffer.data() + i, "/", 1) == 0){ // sets the pointer to the first trailer object
                dict_offset = i + 1;
                break;
            }
        }
    }
    if(dict_offset == 0) throw std::runtime_error("Error: missing '<<' in the trailer section.");

    for(size_t i = dict_offset; i <= file_size; i++){ // check for trailer dict opening brackets
        if(std::memcmp(buffer.data() + i, ">>", 2) == 0){
            dict_end = i;
            break;
        }
    }
    if(dict_end == 0) throw std::runtime_error("Error: missing '>>' in the trailer section.");

    std::string dict_buffer(buffer.begin() + dict_offset, buffer.begin() + dict_end);
    std::istringstream dict_stream(dict_buffer);

    std::string token, key, val;

    while(!dict_stream.eof()){
        std::getline(dict_stream, token, ' ');
        key = "/" + token;

        std::getline(dict_stream, token, '/');
        while(!token.empty() && std::isspace(static_cast<unsigned char>(token.back()))){
            token.pop_back();
        }
        val = token;

        pdf_doc.get_trailer().set(key, val);
    }
}

void Parser::parse_objects(std::ifstream& file, PDFDocument &pdf_doc){
    for(auto &table : pdf_doc.get_xref_tables().tables){
        for(auto &entry : table.entries){
            if(entry.inUse == 'n'){
                auto obj = std::make_unique<Object>();
                obj->offset = entry.offset;

                uint32_t id;
                uint16_t gen;
                std::string content, trash;

                file.seekg(obj->offset);
                file >> id >> gen >> trash;
                content = stream_until_keyword(file, "endobj");

                obj->id = id;
                obj->gen = gen;
                obj->content = content;

                pdf_doc.get_objects().push_back(std::move(obj));
            }
        }
    }
}

PDFDocument Parser::parsePDF(const std::string &path){
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: cannot open file");
    }

    std::cout << "File opened successfully." << "\n\n";

    uint64_t xref_offset = find_xref_offset(file);

    std::cout << "xref table offset: " << xref_offset << "\n\n";

    PDFDocument pdf_doc;

    read_xref_table(file, pdf_doc, xref_offset);

    parse_trailer(file, pdf_doc);

    parse_objects(file, pdf_doc);

    return pdf_doc;
}