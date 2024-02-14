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


int PlayerData::debugStr(char* buffer) {
    // OSReport("buffer: 0x%X\n", buffer);
    // OSReport("actionname: 0x%X\n", actionname);
    // OSReport("subactionName: 0x%X\n", subactionName);
    PlayerDataOnFrame& f = *(this->current);

    return snprintf(buffer, WISP_STR_MANIP_SIZE,
    "  Action: 0x%X, %s\n"
    "  Subaction: 0x%X, %s\n"
    "  Frames: %d/%d\n"
    "  Hitstun: %d/%d\n"
    "  Shieldstun?: %d/%d\n"
    "  Shielding: %c\n"
    ,
    f.action, actionName(f.action),
    f.subaction, f.subactionName,
    f.currentFrame, f.totalFrames,
    f.hitstun, maxHitstun,
    f.shieldstun, maxShieldstun,
    (f.isShielding() ? 'T' : 'F')
    );
}
