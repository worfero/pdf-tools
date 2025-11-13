#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string stream_until_keyword(std::ifstream &file, std::string keyword);

#endif