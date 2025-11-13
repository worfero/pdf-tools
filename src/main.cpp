#include <iostream>
#include "pdf-tools.hpp"

int main(){
    PDFTools tools;

    tools.copyPDF("./pdfs/newpdf.pdf", "./pdfs/copy.pdf");

    return 0;
}