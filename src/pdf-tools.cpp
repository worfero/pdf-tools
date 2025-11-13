#include "pdf-tools.hpp"

void PDFTools::copyPDF(const std::string &src_path, const std::string &dest_path){
    PDFDocument pdf_doc;

    pdf_doc = parser.parsePDF(src_path);

    builder.build_pdf(pdf_doc, dest_path);
}