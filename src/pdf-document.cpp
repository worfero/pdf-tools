#include "pdf-document.hpp"

void PDFDocument::add_xref_entry(const X_Ref_Entry &entry) {
    xref_table.push_back(entry);
}

std::vector<X_Ref_Entry> PDFDocument::get_xref_table() {
    return xref_table;
}