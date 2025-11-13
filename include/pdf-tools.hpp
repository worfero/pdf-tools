#ifndef PDF_TOOLS_H
#define PDF_TOOLS_H

#include <string>

#include "pdf-manip/pdf-document.hpp"
#include "pdf-manip/parser.hpp"
#include "pdf-manip/build-pdf.hpp"

class PDFTools{
private:
    Parser parser;
    Builder builder;
public:
    void copyPDF(const std::string &src_path, const std::string &dest_path);
};

#endif