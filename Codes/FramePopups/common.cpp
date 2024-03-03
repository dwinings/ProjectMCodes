#include "./common.h"
/********/
// Universal Externs. This is imported by common.h so it is usable throughout.

u32 frameCounter = 0;
char strManipBuffer[WISP_STR_MANIP_SIZE] = {};

/******/

float fmax(float x1, float x2) {
    if (x1 > x2) return x1;
    return x2;
}

int max(int x1, int x2) {
    if (x1 > x2) return x1;
    return x2;
}