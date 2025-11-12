#include <iostream>
#include "parser.hpp"
#include "pdf-document.hpp"

int main(){
    Parser parser;
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF("./pdfs/newpdf.pdf");

    std::cout << "X-Ref Table: " << "\n";
    pdf_doc.get_xref_tables().print_entries();

    std::cout << "\n" << "Trailer: " << "\n";
    pdf_doc.get_trailer().print_dict();

    std::cout << "\n" << "Objects: " << "\n";
    pdf_doc.print_objects();

    return 0;
}