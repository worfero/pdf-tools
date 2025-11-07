#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>
#include "xref-table.hpp"

#define XREF_OFFSET_ERROR 0
#define LOOKBACK 4096 // 4kb of data

void parsePDF(std::string pdf);

uint64_t find_xref_offset(std::ifstream& file);

std::vector<X_Ref_Entry> read_xref_table(std::ifstream &file, uint64_t xref_offset);

#endif