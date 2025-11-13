#include "pdf-document.hpp"

void PDFDocument::print_objects(){
    for(auto &obj : objects){
        std::cout << "[" << "\n";
        std::cout << obj->id << " " << obj->gen << " obj";
        std::cout << obj->content;
        std::cout << "]" << "\n\n";
    }
};

//getters

Header &PDFDocument::get_header() {
    return header;
};

const Header &PDFDocument::get_header() const {
    return header;
};

X_Ref_Tables &PDFDocument::get_xref_tables() {
    return xref_tables;
};

const X_Ref_Tables &PDFDocument::get_xref_tables() const {
    return xref_tables;
};

Trailer &PDFDocument::get_trailer() {
    return trailer;
};

const Trailer &PDFDocument::get_trailer() const {
    return trailer;
};

std::vector<std::unique_ptr<Object>> &PDFDocument::get_objects() {
    return objects;
};

const std::vector<std::unique_ptr<Object>> &PDFDocument::get_objects() const {
    return objects;
};