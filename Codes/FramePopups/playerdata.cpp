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


int PlayerDataOnFrame::debugStr(char* buffer) {
    return snprintf(buffer, WISP_STR_MANIP_SIZE,
    "  Action: 0x%X, %s\n"
    "  Subaction: 0x%X, %s (%d/%d)\n"
    "  Frames: %d/%d\n"
    "  Hitstun: %d"
    "  Shielding: %s\n"
    ,
    action, actionname,
    subaction, subactionName, subactionFrame, subactionEndFrame,
    currentFrame, totalFrames,
    hitstun,
    isShielding() ? "T" : "F"
    );
}
