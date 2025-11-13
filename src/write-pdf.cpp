#include "write-pdf.hpp"

void write_pdf(PDFDocument pdf_doc, const std::string& path){
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: cannot open file");
    }

    
}