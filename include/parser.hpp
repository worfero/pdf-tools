#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdint>
#include "pdf-objects.hpp"
#include "pdf-document.hpp"

#define LOOKBACK 4096 // 4kb of data

class Parser{
private:
    // xref table methods
    uint64_t find_xref_offset(std::ifstream& file);
    void read_xref_table(std::ifstream &file, PDFDocument &pdf_doc, uint64_t xref_offset);

    // trailer methods
    void parse_trailer(std::ifstream& file, PDFDocument &pdf_doc);
    
public:
    PDFDocument parsePDF(const std::string &path);

};

#endif