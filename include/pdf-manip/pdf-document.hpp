#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

#include <vector>
#include <map>
#include <string>
#include "pdf-objects.hpp"

class PDFDocument {
private:
    Header header;
    X_Ref_Tables xref_tables;
    Trailer trailer;
    std::vector<std::unique_ptr<Object>> objects;

public:
    PDFDocument() = default;

    void print_objects();

    // getters
    Header &get_header();
    const Header &get_header() const;

    std::vector<std::unique_ptr<Object>> &get_objects();
    const std::vector<std::unique_ptr<Object>> &get_objects() const;

    X_Ref_Tables &get_xref_tables();
    const X_Ref_Tables &get_xref_tables() const;

    Trailer &get_trailer();
    const Trailer &get_trailer() const;
};

#endif