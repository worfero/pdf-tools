#ifndef XREF_H
#define XREF_H

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

struct X_Ref_Entry {
    uint64_t offset;
    uint16_t generation;
    bool inUse;
};

#endif