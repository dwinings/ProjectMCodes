#ifndef WISP_PLAYERDATA_H
#define WISP_PLAYERDATA_H

#include <CLibs/cstring.h>
#include <Memory.h>

#include "actions.h"
#include "common.h"

bool startsWith(const char* testStr, const char* prefix);

// This is stuff that changes on every frame.
struct PlayerDataOnFrame {
    u32 action = 0;
    u32 subaction = 0;
    char subactionName[32];
    float subactionFrame = 0;
    float subactionTotalFrames = 0;

    u32 lowRABits = 0;
    u16 actionFrame = 0;
    u16 actionTotalFrames = 0;
    u16 hitstun = 0;
    u16 shieldstun = 0;

    u32 canCancel: 1 = false;
    u32 didConnectAttack: 1 = false;

    bool getLowRABit(u32 idx);
    inline bool isShielding();

};

// This is mostly meta-data that we track over multiple frames.
// It doesn't auto-clear and needs to be cleared manually.
struct PlayerData {
    public:
        u16 maxHitstun = 0;
        u16 maxShieldstun = 0;
        u8 playerNumber;
        CHAR_ID charKind;

        /* Stuff for on-shield targeting bookkeeping*/
        PlayerData* attackTarget = nullptr;
        char advantageBonusCounter = 0;
        u32 becameActionableOnFrame = -1;
        u32 lastAttackEndedOnFrame = -1;
        u16 attackingAction = -1;
        /*******/

        /* Swapped every frame */
        PlayerDataOnFrame* prev = &_f1;
        PlayerDataOnFrame* current = &_f2;

        /* flags */
        unsigned int didStartAttack: 1 = false;
        unsigned int didConnectAttack: 1 = false;

        void resetTargeting();

        /* aliases for fields on Current*/
        inline u32 action();
        inline u16 actionFrame();
        inline u16 actionTotalFrames();
        inline u32 subaction();
        inline float subactionFrame();
        inline float subactionTotalFrames();


        inline bool didReceiveHitstun();
        inline bool didReceiveShieldstun();
        inline bool didBecomeActionable();
        inline bool didEnableCancel();
        inline bool didActionChange();
        inline bool didSubactionChange();
        inline bool didEnterShield();
        inline void prepareNextFrame();

        bool resolvePlayerActionable();
        bool resolveTargetActionable();
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

inline u32 PlayerData::action() {
    return this->current->action;
}

inline u32 PlayerData::subaction() {
    return this->current->subaction;
}

inline u16 PlayerData::actionFrame() {
    return this->current->actionFrame;
}

inline u16 PlayerData::actionTotalFrames() {
    return this->current->actionTotalFrames;
}

inline float PlayerData::subactionFrame() {
    return this->current->subactionFrame;
}

inline float PlayerData::subactionTotalFrames() {
    return this->current->subactionTotalFrames;
}

inline bool PlayerData::didReceiveHitstun() {
        return current->hitstun != 0 && (current->hitstun > prev->hitstun);
}

inline bool PlayerData::didReceiveShieldstun() {
        return current->shieldstun != 0 && (current->shieldstun > prev->shieldstun);
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
        || current->actionFrame != (prev->actionFrame + 1)
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


extern PlayerData allPlayerData[WISP_MAX_PLAYERS];

#endif