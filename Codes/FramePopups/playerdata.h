#ifndef WISP_PLAYERDATA_H
#define WISP_PLAYERDATA_H

#include <CLibs/cstring.h>
#include <Memory.h>

#include "actions.h"
#include "common.h"

bool startsWith(const char* testStr, const char* prefix);

struct PlayerDataOnFrame;
struct PlayerData;
struct PlayerDisplayOptions;

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

    bool getLowRABit(u32 idx) const;
    inline bool isShielding() const;

};

// This is mostly meta-data that we track over multiple frames.
// It doesn't auto-clear and needs to be cleared manually.
struct PlayerData {
    public:
        u16 maxHitstun = 0;
        u16 maxShieldstun = 0;
        u8 playerNumber;
        CHAR_ID charKind;

        /* Targeting bookkeeping */
        PlayerData* attackTarget = nullptr;
        char advantageBonusCounter = 0;
        u32 becameActionableOnFrame = -1;
        u32 lastAttackEndedOnFrame = -1;
        u16 attackingAction = -1;

        /* Swapped every frame */
        PlayerDataOnFrame* prev = &_f1;
        PlayerDataOnFrame* current = &_f2;

        /* interactive flags */
        u32 didStartAttack: 1 = false;
        u32 didConnectAttack: 1 = false;

        /* display flags */
        bool showOnHitAdvantage = false;
        bool showOnShieldAdvantage = true;
        bool showActOutOfLag = true;


        /* aliases for fields on Current*/
        u16 action() const { return (u16)this->current->action; };
        const char* actionStr() const { return actionName(this->action()); }
        inline u16 actionFrame() const { return current->actionFrame; };
        inline u16 actionTotalFrames() const { return current->actionTotalFrames; };
        inline u32 raLowBits() const { return current->lowRABits; };

        inline u16 subaction() const { return current->subaction; };
        inline const char* subactionStr() const { return current->subactionName; }
        inline float subactionFrame() const { return current->subactionFrame; };
        inline float subactionTotalFrames() const { return current->subactionTotalFrames; };

        /* Derived information. */
        inline bool didReceiveHitstun() const;
        inline bool didReceiveShieldstun() const;
        inline bool didBecomeActionable() const;
        inline bool didEnableCancel() const;
        inline bool didActionChange() const;
        inline bool didSubactionChange() const;
        inline bool didEnterShield() const;
        inline void prepareNextFrame();

        /* Lifecycle methods */
        void resetTargeting();
        bool resolvePlayerActionable();
        bool resolveTargetActionable();
        int debugStr(char* buffer);
    private: 
        PlayerDataOnFrame _f1;
        PlayerDataOnFrame _f2;
};

struct PlayerDisplayOptions {
};


inline bool PlayerData::didEnterShield() const {
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

inline bool PlayerData::didReceiveHitstun() const {
        return current->hitstun != 0 && (current->hitstun > prev->hitstun);
}

inline bool PlayerData::didReceiveShieldstun() const {
        return current->shieldstun != 0 && (current->shieldstun > prev->shieldstun);
}

inline bool PlayerData::didBecomeActionable() const {
    return true;
}

inline bool PlayerData::didEnableCancel() const {
    return current->canCancel && !(prev->canCancel);
}

inline bool PlayerData::didActionChange() const {
    return (
        (current->action != prev->action)
        || current->actionFrame != (prev->actionFrame + 1)
    );
}

inline bool PlayerData::didSubactionChange() const {
    return (!prev->isShielding() && current->isShielding());
}

inline bool PlayerDataOnFrame::isShielding() const {
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