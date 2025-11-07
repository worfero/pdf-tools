#include "pdf-document.hpp"

X_Ref_Table &PDFDocument::get_xref_table() {
    return xref_table;
};

const X_Ref_Table &PDFDocument::get_xref_table() const {
    return xref_table;
};