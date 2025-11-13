#ifndef WRITE_PDF_H
#define WRITE_PDF_H

#include <iostream>
//#include <vector>
//#include <iomanip>
#include <fstream>
#include <string>
//#include <sstream>
//#include <cstring>
//#include <cstdint>
//
//#include "pdf-objects.hpp"
#include "pdf-document.hpp"
//#include "utils.hpp"

void write_pdf(PDFDocument pdf_doc, const std::string& path);

#endif