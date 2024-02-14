#ifndef WISP_COMMON_H
#define WISP_COMMON_H

#define WISP_STR_MANIP_SIZE 512
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define snprintf ((int (*)(char* buffer, size_t maxSize, const char* format, ...)) 0x803f8924)

#include "actions.h"

typedef struct Coord2D {
    int x = 0;
    int y = 0;
} Coord2D;

#endif