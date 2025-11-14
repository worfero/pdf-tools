#include "pdf-manip/parser.hpp"

void Parser::parse_header(std::ifstream &file, PDFDocument &pdf_doc){
    file.seekg(0, std::ios::beg);
    
    std::string token;
    uint64_t offset;

    file >> token;
    while(token.find('%') == 0){
        offset = static_cast<uint64_t>(file.tellg());
        pdf_doc.get_header().set(token, offset);
        file >> token;
    }
}

uint64_t Parser::find_xref_offset(std::ifstream &file){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_stream_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    
    size_t file_size = static_cast<size_t>(file_stream_size);

    size_t read_size = std::min<size_t>(LOOKBACK, file_stream_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(read_size); // declare char buffer to read the section
    
    file.seekg(file_size - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
    file.read(buffer.data(), read_size); // effectively read the last 4kb of the file

    for(size_t i = read_size - 9; i >= 0; --i){ // "startxref" is 9 characters, so the start index must be moved 9 bytes backwards
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
        uint32_t first_entry, count;
        file >> token;
        if(!isdigit(token[0])){
            break;
        }

        first_entry = std::stoi(token);
        file >> count;

        X_Ref_Table xref_table;
        xref_table.first_entry = first_entry;
        xref_table.count = count;

        for(uint32_t i = 0; i < count; i++){
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

uint64_t Parser::find_trailer_offset(std::ifstream& file){
    file.seekg(0, std::ios::end); // move the stream pointer to the end of the file
    std::streampos file_stream_size = file.tellg(); // get the current position of the pointer as a byte size in relation to the full size
    
    size_t file_size = static_cast<size_t>(file_stream_size);

    size_t read_size = std::min<size_t>(LOOKBACK, file_stream_size); // determine the maximum read size to be the LOOKBACK directive
    std::vector<char> buffer(file_stream_size); // declare char buffer to read the section

    file.seekg(file_size - read_size, std::ios::beg); // moves the pointer 4kb before the end of the file
    file.read(buffer.data(), read_size); // effectively read the last 4kb of the file

    // trailer offset relative to the ending 4kb of the file
    size_t trailer_offset = 0;

    for(size_t i = read_size - 7; i >= 0; i--){ // "trailer" is 7 characters, so the start index must be moved 7 bytes backwards
        if(std::memcmp(buffer.data() + i, "trailer", 7) == 0){ // compares the last 7 bytes of the buffer with "trailer"
            trailer_offset = i;
            break;
        }
    }
    if(trailer_offset == 0) throw std::runtime_error("Error: trailer not found.");

    return static_cast<uint64_t>(trailer_offset);
}

void Parser::parse_trailer(std::ifstream& file, PDFDocument &pdf_doc){
    uint64_t trailer_offset = find_trailer_offset(file);

    file.seekg(trailer_offset + 7);

    std::string trailer_content = stream_until_keyword(file, "startxref");

    bool has_opn_brk = false;
    bool has_cls_brk = false;
    size_t i = 0;
    while (i < trailer_content.size()) {
        if((isspace(trailer_content[i]) && i == 0) || (trailer_content[i] == '<' || trailer_content[i] == '>')){
            if(trailer_content[i] == '<' && trailer_content[i+1] == '<'){
                has_opn_brk = true;
            }
            if(trailer_content[i] == '>' && trailer_content[i+1] == '>'){
                has_cls_brk = true;
            }
            trailer_content.erase(i, 1);
        }
        else{
            i++;
        }
    }
    if(!has_opn_brk) throw std::runtime_error("Error: missing '<<' in the trailer section.");
    if(!has_cls_brk) throw std::runtime_error("Error: missing '>>' in the trailer section.");

    std::map<std::string, std::string> parsed_dict = parse_dict(trailer_content);

    pdf_doc.get_trailer().dict.replace(parsed_dict);

    if(pdf_doc.get_trailer().get_root_id() == 0) throw std::runtime_error("Error: '/Root' object reference not found.");
}

void Parser::parse_objects(std::ifstream& file, PDFDocument &pdf_doc){
    uint32_t rootID = pdf_doc.get_trailer().get_root_id();

    for(auto &table : pdf_doc.get_xref_tables().tables){
        for(auto &entry : table.entries){
            std::unique_ptr<Object> obj;

            uint32_t id;
            uint16_t gen;
            std::string content, trash;

            if(entry.inUse == 'n'){
                file.seekg(entry.offset);
                file >> id >> gen >> trash;
                if(id == rootID){
                    obj = std::make_unique<RootObject>();
                }
                else{
                    obj = std::make_unique<Object>();
                }
                
                obj->offset = entry.offset;
                obj->id = id;
                obj->gen = gen;
                content = stream_until_keyword(file, "endobj");
                obj->content = content;

                if(id == rootID){
                    RootObject* root = dynamic_cast<RootObject*>(obj.get());
                    size_t i = 0;
                    while (i < root->content.size()) {
                        if((isspace(root->content[i]) && i == 0) || (root->content[i] == '<' || root->content[i] == '>')){
                            root->content.erase(i, 1);
                        }
                        else{
                            i++;
                        }
                    }
                    std::map<std::string, std::string> parsed_dict = parse_dict(root->content);
                    root->dict.replace(parsed_dict);
                }
            }
            else{
                obj = std::make_unique<Object>();
                obj->offset = 0;
                obj->id = 0;
                obj->gen = entry.generation;
                obj->content.clear();
            }

            obj->inUse = entry.inUse;
            obj->xref_entry = &entry;

            pdf_doc.get_objects().push_back(std::move(obj));
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

    parse_header(file, pdf_doc);

    read_xref_table(file, pdf_doc, xref_offset);

    parse_trailer(file, pdf_doc);

    parse_objects(file, pdf_doc);

    return pdf_doc;
}