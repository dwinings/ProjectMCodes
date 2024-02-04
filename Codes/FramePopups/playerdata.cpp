#include "playerdata.h"


bool startsWith(const char* testStr, const char* prefix) {
    while (*prefix != '\0') {
        if (*testStr == '\0') {
            return false;
        }

        if (*prefix != *testStr) {
            return false;
        }

        prefix++;
        testStr++;
    }
    return true;
}
