#ifndef WISP_PLAYERDATA_H
#define WISP_PLAYERDATA_H

#include <CLibs/cstring.h>
#include <Memory.h>
#include <stddef.h>

#include "actions.h"
#include "common.h"

bool startsWith(const char* testStr, const char* prefix);

// This is stuff that changes on every frame.
struct PlayerDataOnFrame {
    u32 action = 0;
    u32 subaction = 0;
    char subactionName[32];
    float subactionFrame = 0;
    float subactionEndFrame = 0;

    u16 currentFrame = 0;
    u16 totalFrames = 0;
    u16 hitstun = 0;
    u16 shieldstun = 0;
    bool canCancel = 0;

    inline bool isShielding();

};

// This is mostly meta-data that we track over multiple frames.
// It doesn't auto-clear and needs to be cleared manually.
struct PlayerData {
    public:
        u16 maxHitstun = 0;
        u16 maxShieldstun = 0;
        u8 playerNumber;

        /* Stuff for on-shield targeting bookkeeping*/
        PlayerData* attackTarget = nullptr;
        bool didConnectAttack = 0;
        char advantageBonusCounter = 0;
        u32 becameActionableOnFrame = -1;
        u16 attackingAction = -1;
        /*******/

        /* Swapped every frame */
        PlayerDataOnFrame* prev = &_f1;
        PlayerDataOnFrame* current = &_f2;

        void resetTargeting();
        inline bool didReceiveHitstun();
        inline bool didReceiveShieldstun();
        inline bool didBecomeActionable();
        inline bool didEnableCancel();
        inline bool didActionChange();
        inline bool didSubactionChange();
        inline bool didEnterShield();
        inline void prepareNextFrame();
        int debugStr(char* buffer);
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
}

inline bool PlayerData::didReceiveShieldstun() {
        return current->shieldstun != 0 && (current->shieldstun != prev->shieldstun - 1);
}

inline bool PlayerData::didBecomeActionable() {
    return true;
}

inline bool PlayerData::didEnableCancel() {
    return current->canCancel && !(prev->canCancel);
}

inline bool PlayerData::didActionChange() {
    return (
        (current->action != prev->action)
        || current->currentFrame != (prev->currentFrame + 1)
    );
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