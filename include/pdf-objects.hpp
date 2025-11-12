#ifndef PDF_OBJECTS_H
#define PDF_OBJECTS_H

#include <cstdint>
#include <map>
#include <string>
#include <iostream>

struct X_Ref_Entry {
    uint64_t offset;
    uint16_t generation;
    char inUse;
};

struct X_Ref_Table {
    std::vector<X_Ref_Entry> entries;

    size_t size() const { return entries.size(); }

    void add_entry(const X_Ref_Entry &entry) { entries.push_back(entry); }

    void print_entries() {
        std::cout << "[" << "\n";
        for(size_t i = 0; i < entries.size(); i++){
            std::cout << " " << entries[i].offset << " " << entries[i].generation << " " << entries[i].inUse << "\n";
        }
        std::cout << "]" << "\n";
    }
};

struct Trailer {
    std::map<std::string, std::string> dict;

    void set(const std::string &key, const std::string &value) { dict[key] = value; }

    void print_dict(){
        std::cout << "[" << "\n";
        for(const auto& [k, v] : dict){
            std::cout << k << ", " << v << "\n";
        }
        std::cout << "]" << "\n";
    }
};

#endif