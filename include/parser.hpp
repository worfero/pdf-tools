#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>
#include "pdf-objects.hpp"
#include "pdf-document.hpp"

#define LOOKBACK 4096 // 4kb of data

class Parser{
private:
    uint64_t find_xref_offset(std::ifstream& file);
    void read_xref_table(std::ifstream &file, PDFDocument &pdf_doc, uint64_t xref_offset);
    
public:
    PDFDocument parsePDF(const std::string &path);

};

#endif