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
    OSReport("buffer: 0x%X\n", buffer);
    OSReport("actionname: 0x%X\n", actionname);
    OSReport("subactionName: 0x%X\n", subactionName);

    return snprintf(buffer, WISP_STR_MANIP_SIZE,
    "  Action: 0x%X, %s\n"
    "  Subaction: 0x%X, %s (%0.2f/%0.2f)\n"
    "  Frames: %d/%d\n"
    "  Hitstun: %d"
    "  Shielding: %c\n"
    ,
    action, actionname,
    subaction, subactionName, subactionFrame, subactionEndFrame,
    (u32)currentFrame, (u32)totalFrames,
    (u32)hitstun,
    (isShielding() ? 'T' : 'F')
    );
}
