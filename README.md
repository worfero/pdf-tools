# pdf-tools

PDF tools library for C++, developed as a C++ practice project and .pdf format studying.
Still in (very) early development.

### Tools

For now, it can only parse a pdf and build a new file out of the parsed pdf, but the objects' contents are not actually parsed and are instead treated as a big string.

You can ahieve this by calling "copyPDF(src_path, dest_path)" from the "PDFTools" class.