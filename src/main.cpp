#include <iostream>
#include "parser.hpp"
#include "pdf-document.hpp"

int main(){
    Parser parser;
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF("./pdfs/pdf-minimal.pdf");

    pdf_doc.get_xref_table().print_entries();

    return 0;
}