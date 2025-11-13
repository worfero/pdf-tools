#ifndef BUILD_PDF_H
#define BUILD_PDF_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "pdf-document.hpp"

#define CURRENT_OFFSET static_cast<uint64_t>(file.tellp())

class Builder{
private:

public:
    void build_pdf(PDFDocument &pdf_doc, const std::string &path);
};

#endif