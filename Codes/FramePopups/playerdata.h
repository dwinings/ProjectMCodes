#ifndef WISP_PLAYERDATA_H
#define WISP_PLAYERDATA_H

#include "CLibs/cstring.h"
#include "Memory.h"
#include "stddef.h"

bool startsWith(const char* testStr, const char* prefix);

struct PlayerDataOnFrame {
    char subactionName[32];
    char actionname[32];
    u16 currentFrame = 0;
    u16 totalFrames = 0;
    u16 hitstun = 0;

    inline bool isShielding();
};

struct PlayerData {
    public:
        u16 maxHitstun = 0;
        PlayerDataOnFrame* prev = &_f1;
        PlayerDataOnFrame* current = &_f2;

        inline bool didReceiveHitstun();
        inline bool didBecomeActionable();
        inline bool didActionChange();
        inline bool didSubactionChange();
        inline bool didEnterShield();
        inline void prepareNextFrame();
    private: 
        PlayerDataOnFrame _f1;
        PlayerDataOnFrame _f2;
};

inline bool PlayerData::didEnterShield() {
    if (didSubactionChange()) {
        if (
            startsWith(
                current->subactionName,
                "Guard"
            )
        ) {
            return true;
        }
    }
    return false;
}

inline bool PlayerData::didReceiveHitstun() {
        return current->hitstun != 0 && (current->hitstun != prev->hitstun - 1);
    };

inline bool PlayerData::didBecomeActionable() {
    return true;
}

inline bool PlayerData::didActionChange() {
    if (
        prev->actionname == nullptr ||
        current->actionname == nullptr
    ) {
        return false;
    }

    return strcmp(current->actionname, prev->actionname) != 0;

}

inline bool PlayerData::didSubactionChange() {
    return (!prev->isShielding() && current->isShielding());
}

inline bool PlayerDataOnFrame::isShielding() {
    return startsWith(subactionName, "Guard");
}

inline void PlayerData::prepareNextFrame() {
    PlayerDataOnFrame* tmp = current;

    current = prev;
    prev = tmp;

    memset(current, 0, sizeof(PlayerDataOnFrame));
}

#endif