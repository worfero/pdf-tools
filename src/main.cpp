#include <iostream>
#include "pdf-manip/parser.hpp"
#include "pdf-manip/pdf-document.hpp"
#include "pdf-manip/build-pdf.hpp"

int main(){
    Parser parser;
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF("./pdfs/newpdf.pdf");

    build_pdf(pdf_doc, "./pdfs/copy.pdf");

    return 0;
}