#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

#define XREF_ADDR_ERROR 0
#define LOOKBACK 4096 // 4kb of data

void parsePDF(std::string pdf);

uint64_t find_xref_addr(std::ifstream& file);

#endif