#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

#include <vector>
#include <map>
#include <string>
#include "pdf-objects.hpp"

class PDFDocument {
private:
    X_Ref_Table xref_table;
    //Trailer trailer;

public:
    PDFDocument() = default;

    // getters
    X_Ref_Table &get_xref_table();
    const X_Ref_Table &get_xref_table() const;
};

#endif