#include <iostream>
#include "parser.hpp"
#include "pdf-document.hpp"
#include "build-pdf.hpp"

int main(){
    Parser parser;
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF("./pdfs/Condominio 10_2025.pdf");

    build_pdf(pdf_doc, "./pdfs/copy.pdf");

    return 0;
}