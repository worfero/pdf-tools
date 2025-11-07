#ifndef PDF_OBJECTS_H
#define PDF_OBJECTS_H

#include <cstdint>

struct X_Ref_Entry {
    uint64_t offset;
    uint16_t generation;
    char inUse;
};

#endif