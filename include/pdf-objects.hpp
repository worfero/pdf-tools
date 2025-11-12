#ifndef PDF_OBJECTS_H
#define PDF_OBJECTS_H

#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <memory>

struct Object {
    uint32_t id;
    uint16_t gen;
    uint64_t offset;
    std::string content;
    virtual ~Object() = default;
};

struct X_Ref_Entry {
    uint64_t offset;
    uint16_t generation;
    char inUse;
};

struct X_Ref_Table {
    uint16_t first_entry;
    size_t count;
    std::vector<X_Ref_Entry> entries;

    void add_entry(const X_Ref_Entry &entry) { entries.push_back(entry); }
};

struct X_Ref_Tables {
    std::vector<X_Ref_Table> tables;

    size_t size() const { 
        size_t size = 0;
        for(X_Ref_Table table : tables){
            size += table.entries.size();
        }
        return size;
    }

    void add_table(X_Ref_Table &table) { tables.push_back(table); }

    void print_entries() {
        std::cout << "[" << "\n";
        for(X_Ref_Table table : tables){
            std::cout << table.first_entry << " " << table.count << ":" << "\n";
            std::cout << "[" << "\n";
            for(X_Ref_Entry entry : table.entries){
                std::cout << entry.offset << " " << entry.generation << " " << entry.inUse << "\n";
            }
            std::cout << "]," << "\n\n";
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