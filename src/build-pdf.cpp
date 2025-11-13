#include "build-pdf.hpp"

#define CURRENT_OFFSET static_cast<uint64_t>(file.tellp())

void build_pdf(PDFDocument &pdf_doc, const std::string &path){
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: cannot open file");
    }

    for(auto &[ header, offset ] : pdf_doc.get_header().data){
        file << header << "\n";
    }

    for(auto &obj : pdf_doc.get_objects()){
        if(obj->inUse == 'n'){
            obj->offset = CURRENT_OFFSET;
            obj->xref_entry->offset = obj->offset;
            file << obj->id << " " << obj->gen << " obj\n" << obj->content << "\n" << "endobj\n";
        }
    }

    uint64_t xref_offset = CURRENT_OFFSET;
    file << "xref\n";

    for(auto &table : pdf_doc.get_xref_tables().tables){
        file << table.first_entry << " " << table.count << "\n";
        for(auto &entry : table.entries){
            file << std::setw(10) << std::setfill('0') << entry.offset << " "
            << std::setw(5) << std::setfill('0') << entry.generation << " " << entry.inUse << "\n";
        }
    }

    file << "trailer\n<< ";
    for(auto &[key, value] : pdf_doc.get_trailer().dict){
        file << key << " " << value << "\n";
    }
    file << ">>\n";

    file << "startxref\n" << xref_offset << "\n";

    file << "%%EOF";
}