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

std::map<std::string, std::string> parse_dict(std::string dict_buffer){
    std::istringstream dict_stream(dict_buffer);
    std::map<std::string, std::string> dict;
    
    std::string token, key, val;
    while(!dict_stream.eof()){
        std::getline(dict_stream, token, ' ');
        if(token[0] != '/'){
            key = "/";
        }
        key += token;

        std::getline(dict_stream, token, '/');
        while(!token.empty() && std::isspace(static_cast<unsigned char>(token.back()))){
            token.pop_back();
        }
        val = token;

        dict[key] = val;
    }
    return dict;
}