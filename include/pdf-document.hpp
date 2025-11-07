#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

#include <vector>
#include "pdf-objects.hpp"

class PDFDocument {
private:
    std::vector<X_Ref_Entry> xref_table;

public:
    PDFDocument() = default;

    void add_xref_entry(const X_Ref_Entry &entry);
    std::vector<X_Ref_Entry> get_xref_table();
};

#endif