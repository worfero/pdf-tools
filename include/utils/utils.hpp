#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

std::string stream_until_keyword(std::ifstream &file, std::string keyword);

std::map<std::string, std::string> parse_dict(std::string dict_buffer);

#endif