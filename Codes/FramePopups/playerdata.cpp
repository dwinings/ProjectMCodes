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

void PlayerData::resetTargeting() {
    this->attackingAction = -1;
    this->attackTarget = nullptr;
    this->becameActionableOnFrame = -1;
    this->advantageBonusCounter = 0;
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

bool PlayerDataOnFrame::getLowRABit(u32 idx) {
    if (idx >= 32) {
        OSReport("WARNING: Asked for ra-bit that's too high.\n");
        return false;
    }

    return 1 == ((this->lowRABits & (1 << idx)) >> idx);
}