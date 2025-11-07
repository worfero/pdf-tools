#include <iostream>
#include "parser.hpp"
#include "pdf-document.hpp"

int main(){
    Parser parser;
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF("./pdfs/pdf-minimal.pdf");

    std::vector<X_Ref_Entry> xref_table = pdf_doc.get_xref_table();

    std::cout << "[" << "\n";
    for(size_t i = 0; i < xref_table.size(); i++){
        std::cout << " " << xref_table[i].offset << " " << xref_table[i].generation << " " << xref_table[i].inUse << "\n";
    }
    std::cout << "]" << std::endl;

    return 0;
}