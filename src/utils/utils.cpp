#include "utils/utils.hpp"

std::string stream_until_keyword(std::ifstream &file, std::string keyword){
    std::ostringstream buffer;
    std::string window;
    std::string content;
    char c;

    while (file.get(c)) {
        buffer.put(c);
        window.push_back(c);

        if (window.size() > keyword.size())
            window.erase(0, 1);

        if (window == keyword)
            break;
    }

    content = buffer.str();
    content.erase(content.size() - keyword.size());

    return content;
}