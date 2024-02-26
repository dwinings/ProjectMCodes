
#include "main.h"


// show menu / pause game execution
bool paused = false;
bool visible = false;
int wispMenuXPos = 200;
int wispMenuYPos = 50;
u8 wispAlpha = 255;

char strManipBuffer[WISP_STR_MANIP_SIZE]; 
float wispFontScaleX = 0.45;
float wispFontScaleY = 0.7;
int wispLineHeightMultiplier = 20;
unsigned int frameCounter = 0;

struct PlayerData allPlayerData[WISP_MAX_PLAYERS] = {};
linkedlist<Popup> playerPopups[WISP_MAX_PLAYERS] = {
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>()
};


// responsible for pausing exeuction of the game itself while the code menu is up
INJECTION("TOGGLE_PAUSE", 0x8002E5B0, R"(
    mr r3, r25
    bl checkMenuPaused 
    lwz r3, 0 (r25)
)");

extern "C" void checkMenuPaused(char* gfTaskSchedulerInst) {
    // OSReport("Visible: %s, paused: %s\n", visible ? "T" : "F", paused ? "T" : "F");
    if (paused && visible) { gfTaskSchedulerInst[0xB] |= 0x8; }
    else { gfTaskSchedulerInst[0xB] &= ~0x8; }
}

void printRABools(const soWorkManageModuleImpl& workModule) {
    auto RABoolArr = (*(bool (*)[workModule.RAVariables->bitsSize])workModule.RAVariables->bitVariables);
    bool hasPrintedHeader = false;
    for (int i = 0; i < workModule.RAVariables->bitsSize; i++) {
        char byte = RABoolArr[i];
        for (int j = 0; j < 8; j++) {
            char val = (byte & (0x1 << j)) >> j;
            int boolNum = (i*8) + j;
            if (val != 0){
                if (hasPrintedHeader == false) {
                    OSReport("Player 0 RA bools: \n");
                    hasPrintedHeader = true;
                }
                OSReport("   bit %d: %d\n", boolNum, val);
            }

        }
    }
}

void printLABools(const soWorkManageModuleImpl& workModule) {
    auto LABoolArr = (*(bool (*)[workModule.LAVariables->bitsSize])workModule.LAVariables->bitVariables);
    bool hasPrintedHeader = false;
    for (int i = 0; i < workModule.LAVariables->bitsSize; i++) {
        char byte = LABoolArr[i];
        for (int j = 0; j < 8; j++) {
            char val = (byte & (0x1 << j)) >> j;
            int boolNum = (i*8) + j;
            if (val != 0){
                if (hasPrintedHeader == false) {
                    OSReport("Player 0 LA bools: \n");
                    hasPrintedHeader = true;
                }
                OSReport("   bit %d: %d\n", boolNum, val);
            }

        }
    }
}

bool getRABit(const soWorkManageModuleImpl& workModule, u32 idx) {
    auto RABitsCnt = workModule.RAVariables->bitsSize;
    auto RABitsAry = (*(char (*)[RABitsCnt])workModule.RAVariables->bitVariables);

    if (!(idx < RABitsCnt*8)) {
        OSReport("Warning: asked for invalid RA bit %d from workModule %x.\n", idx, (void*)&workModule);
        return false;
    }

    char bitsChunk = RABitsAry[idx / 8];
    char remainder = idx % 8;
    if (((bitsChunk << remainder) >> remainder) == 0) {
        return false;
    } else {
        return true;
    }
}

void breakpoint() {
    OSReport("hello.\n");
}

void printMessage(char const* msg, float xPos, float yPos, GXColor color = COLOR_WHITE){
    OSReport("%s\n", msg);
    printer.setup();
    printer.setTextColor(color);
    printer.renderPre = true;
    Message* printerMsgObj = &(printer.message);
    printer.lineHeight = wispFontScaleY * wispLineHeightMultiplier;
    printerMsgObj->fontScaleY = wispFontScaleY;
    printerMsgObj->fontScaleX = wispFontScaleX;
    printerMsgObj->xPos = xPos;
    printerMsgObj->yPos = yPos;
    printerMsgObj->zPos = 0;
    printer.start2D();

    printer.startBoundingBox();
    printer.print(msg);
    printer.saveBoundingBox(printer.bboxIdx, COLOR_TRANSPARENT_GREY, WISP_PRINTER_PADDING);
}

void handleInput() {
    PADButtons btn;
    btn.bits = PREVIOUS_PADS[0].button.bits | PREVIOUS_PADS[1].button.bits | PREVIOUS_PADS[2].button.bits | PREVIOUS_PADS[3].button.bits;


    if (btn.L && btn.R && btn.UpDPad) {
        wispAlpha += 1;
        return;
    }

    if (btn.L && btn.LeftDPad) {
        wispMenuXPos = max(0, wispMenuXPos - 1);
        return;
    }
    if (btn.L && btn.RightDPad) {
        wispMenuXPos = wispMenuXPos + 1;
        return;
    }
    if (btn.L && btn.DownDPad) {
        wispMenuYPos = max(0, wispMenuYPos - 1);
        return;
    }
    if (btn.L && btn.UpDPad) {
        wispMenuYPos += 1;
        return;
    }

    if (btn.R && btn.LeftDPad) {
        wispFontScaleX = fmax(0, wispFontScaleX - WISP_FONT_SCALE_INC);
        return;
    }
    if (btn.R && btn.RightDPad) {
        wispFontScaleX += WISP_FONT_SCALE_INC;
        return;
    }
    if (btn.R && btn.DownDPad) {
        wispFontScaleY = fmax(0, wispFontScaleY - WISP_FONT_SCALE_INC);
        return;
    }
    if (btn.R && btn.UpDPad) {
        wispFontScaleY += WISP_FONT_SCALE_INC;
        return;
    }
    if (btn.R && btn.A) {
        wispLineHeightMultiplier += 1;
        return;
    }
    if (btn.R && btn.B) {
        wispLineHeightMultiplier = max(0, wispLineHeightMultiplier - 1);
        return;
    }
}

INJECTION("enable_cancel_transition_group", 0x8084b830, R"(
    SAVE_REGS
    li r3, 0
    or r3, r26, r26
    bl afterEnableCancelTransitionGroup
    RESTORE_REGS
    blr
)");
extern "C" void afterEnableCancelTransitionGroup(ftCancelModule* cancelModule) {
    OSReport("hello. Cancel Module: 0x%x", (void*)cancelModule);
}

// calls our function
INJECTION("update_pre_frame", /*0x8001792c*/ 0x800177B0, R"(
    SAVE_REGS
    bl updatePreFrame
    RESTORE_REGS
    or r0, r0, r3
)");
extern "C" void updatePreFrame() {
    SCENE_TYPE sceneType = (SCENE_TYPE)getScene();

    if (sceneType == SCENE_TYPE::VS || sceneType == SCENE_TYPE::TRAINING_MODE_MMS) {
        frameCounter += 1;
        renderables.renderPre();

        // Lazy debounce.
        if (frameCounter % 3 == 0) {
            handleInput();
        }

        int fighterCount;
        fighterCount = FIGHTER_MANAGER->getEntryCount();
        u8 player = 0;
        for (player = 0; player < fighterCount; player++) {
            gatherData(player);
        }

        for (player = 0; player < fighterCount; player++) {
            resolveAttackTarget(player);
        }

        for (player = 0; player < fighterCount; player++) {
            checkAttackTargetActionable(player);
        }

        drawAllPopups();
    }


    startNormalDraw();
}

void debugWorkModule(const soWorkManageModuleImpl& workModule) {
    u32 i;
    auto RABasicsArr = (*(int(*)[workModule.RAVariables->basicsSize])workModule.RAVariables->basics);
    auto RAFloatArr = (*(int(*)[workModule.RAVariables->floatsSize])workModule.RAVariables->floats);
    auto LABasicsArr = (*(int(*)[workModule.LAVariables->basicsSize])workModule.LAVariables->basics);
    auto LAFloatArr = (*(float(*)[workModule.LAVariables->floatsSize])workModule.LAVariables->floats);

    for(i = 0; i < workModule.RAVariables->basicsSize; i++) {
        if (RABasicsArr[i] != 0 && RABasicsArr[i] != -1){
            OSReport("\tRABasic #%d: %d \n", i, RABasicsArr[i]);
        }
    }

    for (i = 0; i < workModule.RAVariables->floatsSize; i++) {
        if (RAFloatArr[i] != 0 && RAFloatArr[i] != -1) {
            OSReport("\tRAFloat #%d: %0.2f\n", i, RAFloatArr[i]);
        }
    }

    for(i = 0; i < workModule.LAVariables->basicsSize; i++) {
        if (LABasicsArr[i] != 0 && LABasicsArr[i] != -1) {
            OSReport("\tLABasic #%d: %d \n", i, LABasicsArr[i]);
        }
    }

    for (i = 0; i < workModule.LAVariables->floatsSize; i++) {
        if (LAFloatArr[i] != 0 && LAFloatArr[i] != -1) {
            OSReport("\tLAFloat #%d: %0.2f\n", i, LAFloatArr[i]);
        }
    }

    return;
}


void gatherData(u8 player) {
    if (player > 3) {
        OSReport("Asked to gather data for invalid player %d\n", player);
        return;
    }

    PlayerData& playerData = allPlayerData[player];
    playerData.prepareNextFrame();
    PlayerDataOnFrame& currentData = *playerData.current;
    PlayerDataOnFrame& prevData = *playerData.current;

    EntryID entryId = FIGHTER_MANAGER->getEntryIdFromIndex(player);
    Fighter* fighter = FIGHTER_MANAGER->getFighter(entryId, 0);
    u8 playerNumber = FIGHTER_MANAGER->getPlayerNo(entryId);

    allPlayerData[player].playerNumber = playerNumber;
    int character = fighter->getFtKind();

    auto workModule = fighter->modules->workModule;
    auto statusModule = fighter->modules->statusModule;
    auto motionModule = fighter->modules->motionModule;
    auto stopModule = fighter->modules->ftStopModule;
    auto cancelModule = fighter->getCancelModule();

    /*
    OSReport(
        "Player: %d\n"
        "  char type: 0x%X\n" 
        "  entryId: 0x%X\n"
        "  figher addr: 0x%X\n"
        ,
        player, character, entryId, fighter
    );
    */

    // OSReport("Module Locations:\n\tworkModule: %x\n\tstatusModule: %x\n\tmotionModule: %x\n", workModule, statusModule, motionModule);

    if (statusModule != nullptr) {
        /* OSReport("Action number: %x\n", statusModule->action); */
        // never seems to work. strcpy(playerData.current->actionname, statusModule->getStatusName(), WISP_ACTION_NAME_LEN);
        playerData.current->action = statusModule->action;
        if (statusModule->attackHasConnected) {
            playerData.didConnectAttack = true;
        }
    }

    /* hitstun/shieldstun stuff comes from the work module. */
    if (workModule != nullptr) {
        // OSReport("Work module vals for player %d:\n", player);
        // debugWorkModule(*workModule);
        auto RABasicsArr = (*(int(*)[workModule->RAVariables->basicsSize])workModule->RAVariables->basics);
        auto RAFloatArr = (*(int(*)[workModule->RAVariables->floatsSize])workModule->RAVariables->floats);
        auto RABoolArr = (*(bool (*)[workModule->RAVariables->bitsSize])workModule->RAVariables->bitVariables);
        auto LABasicsArr = (*(int(*)[workModule->LAVariables->basicsSize])workModule->LAVariables->basics);
        auto LAFloatArr = (*(float(*)[workModule->LAVariables->floatsSize])workModule->LAVariables->floats);
        auto LABoolArr = (*(bool (*)[workModule->LAVariables->bitsSize])workModule->LAVariables->bitVariables);

        if (playerNumber == 0) {
            /*
            printRABools(*workModule);
            printLABools(*workModule);
            */
        }


        // float shieldValue = LAFloatArr[0x3];

        if (currentData.action == ACTION_GUARDDAMAGE) {
            // This also tracks the lag on shield release frames, which
            // we don't care about for actionability. So we don't want to get this
            // during GuardOff, for example.
            currentData.shieldstun = RABasicsArr[0x5];
        }
        if (currentData.shieldstun != 0) {
            if (playerData.didReceiveShieldstun()) {
                playerData.maxShieldstun = currentData.shieldstun;
                playerData.becameActionableOnFrame = -1;
                playerData.advantageBonusCounter = 0;
            }

            /*
            OSReport("Player %d shieldstun: %d/%d\n",
                playerData.playerNumber, currentData.shieldstun, playerData.maxShieldstun
            );
            */
        } else {
            playerData.maxShieldstun = 0;
        }

        int remainingHitstun = LABasicsArr[0x38];
        playerData.current->hitstun = remainingHitstun;

        if (remainingHitstun != 0) {
            if (playerData.didReceiveHitstun()) {
                playerData.maxHitstun = remainingHitstun;
                playerData.becameActionableOnFrame = -1;
                playerData.advantageBonusCounter = 0;
            }

            /*
            OSReport("Player %d hitstun: %d/%d\n",
                playerData.playerNumber, remainingHitstun, playerData.maxHitstun
            );
            */
        } else {
            playerData.maxHitstun = 0;
        }
  
        /*
        Doesn't trigger
        if (player == 0 && playerData.attackTarget != nullptr && getRABit(*workModule, 0x10)) {
            breakpoint();
        }
        */
    }


    /* subaction stuff */
    if (motionModule != nullptr) {
        soAnimChr animationData = motionModule->mainAnimationData;
        playerData.current->subaction = motionModule->subAction;
        if (animationData.resPtr != nullptr) {
            auto animationResource = animationData.resPtr->CHR0Ptr;
            // OSReport("Animation Resource: 0x%X\n", animationResource);
            if (animationResource == nullptr) {
                strcpy(playerData.current->subactionName, "UNKNOWN", WISP_ACTION_NAME_LEN);
                playerData.current->totalFrames = -1;
            } else {
                playerData.current->subactionFrame = motionModule->getFrame();

                // do these ever differ, except by 1?
                playerData.current->subactionEndFrame = motionModule->getEndFrame();
                playerData.current->totalFrames = animationResource->animLength;
                strcpy(playerData.current->subactionName, animationResource->getString(), WISP_ACTION_NAME_LEN);
            }

            playerData.current->currentFrame = (u32)animationData.animFrame;
        }

    }
    if (cancelModule != nullptr) {
        u32 isEnableCancel = cancelModule->isEnableCancel();
        if (playerNumber == 0) {
            /*
            breakpoint();
            snprintf(strManipBuffer, WISP_STR_MANIP_SIZE, 
                "CancelModule 0x%x, Cancels: %c %c %c %c \n",
                (void*)cancelModule,
                (cancelModule->cancelGroups.bits.cancelGroup1 ? 'T' : 'F'),
                (cancelModule->cancelGroups.bits.cancelGroup2 ? 'T' : 'F'),
                (cancelModule->cancelGroups.bits.cancelGroup3 ? 'T' : 'F'),
                (cancelModule->cancelGroups.bits.cancelGroup4 ? 'T' : 'F')
            );
            printMessage(strManipBuffer, wispMenuXPos, wispMenuYPos, COLOR_WHITE);
            */
        }
        currentData.canCancel = (bool)isEnableCancel;
    }
}

void resolveAttackTarget(u8 playerIdx) {
    PlayerData& player = allPlayerData[playerIdx];
    // False most of the time, so this isn't as slow as it looks.
    if (player.didConnectAttack != false && player.attackTarget == nullptr) {
        for (char otherIdx = 0; otherIdx < WISP_MAX_PLAYERS; otherIdx++) {
            if (playerIdx == otherIdx) {
                continue;
            }

            auto& otherPlayer = allPlayerData[otherIdx];
            if (otherPlayer.didReceiveHitstun() || otherPlayer.didReceiveShieldstun()){
                OSReport("Setting Attacker %d -> Defender %d\n", player.playerNumber, otherPlayer.playerNumber);
                player.attackTarget = &(otherPlayer);
                player.attackingAction = player.current->action;
                break;
            }
        }
    }
}

void checkAttackTargetActionable(u8 playerNum) {
    PlayerData& player = allPlayerData[playerNum];

    // Player is attacking someone.
    if (player.attackTarget != nullptr){
        PlayerData& target = *(player.attackTarget);

        bool playerIsActionable = (
            isDefinitelyActionable(player.current->action)
            || player.current->canCancel
        );

        bool targetIsActionable = (
            (target.current->hitstun == 0 && target.current->shieldstun == 0) 
            || isDefinitelyActionable(target.current->action)
        );

        if (playerIsActionable && player.becameActionableOnFrame == -1) {
            OSReport("Attacker %d became actionable.\n  - Prev Act/Subact: %s/%s\n  - Cur Act/Subact: %s/%s\n", 
                player.playerNumber,
                actionName(player.prev->action), player.prev->subactionName,
                actionName(player.current->action), player.current->subactionName
            );
            player.becameActionableOnFrame = frameCounter;
        }

        if (targetIsActionable && target.becameActionableOnFrame == -1) {
            OSReport("target %d became actionable.\n  - Prev Act/Subact: %s/%s\n  - Cur Act/Subact: %s/%s\n", 
                target.playerNumber,
                actionName(target.prev->action), target.prev->subactionName,
                actionName(target.current->action), target.current->subactionName
            );
            target.becameActionableOnFrame = frameCounter;
        }

        if (playerIsActionable && targetIsActionable) {
            if (target.advantageBonusCounter >= WISP_ADVANTAGE_LENIENCY) {
                /* POP UP TIME! */
                short int advantage = target.becameActionableOnFrame - player.becameActionableOnFrame;

                /* Lots of weird edge cases where the ending doesn't register, such as dying or teching. */
                /* > 30 frames is general judgeable with a human eye anyway. */
                if (advantage > -30 && advantage < 30) {
                    OSReport("Displaying popup for attacker: %d\n", player.playerNumber);
                    snprintf(strManipBuffer, WISP_STR_MANIP_SIZE, "Advantage: %d\n", advantage);
                    OSReport(strManipBuffer);
                    Popup& popup = *(new Popup(strManipBuffer, frameCounter));
                    popup.coords = getHpPopupBoxCoords(player.playerNumber);
                    popup.durationSecs = 3;
                    playerPopups[playerNum].append(popup);
                }

                player.resetTargeting();
                target.resetTargeting();
            } else {
                target.advantageBonusCounter += 1;
            }
        }
    }
}


//hacky way to check if in game
SCENE_TYPE getScene() {
    u32* ptr = (u32*) (0x805b4fd8 + 0xd4);
    ptr = (u32*) *ptr;
    if(ptr < (u32*)0xA0000000) {
        ptr = (u32*) *(ptr + (0x10 / 4));
        if(ptr != nullptr) {
            u32 scene = *(ptr + (8 / 4));
            return (SCENE_TYPE)scene;
        }
    }
    return SCENE_TYPE::UNKNOWN;
}

#define P1_2P_COORDS Coord2D{.x = 200, .y = 350}
#define P2_2P_COORDS Coord2D{.x = 355, .y = 350}
#define P1_4P_COORDS Coord2D{.x = 50, .y = 350}
#define P2_4P_COORDS Coord2D{.x = 200, .y = 350}
#define P3_4P_COORDS Coord2D{.x = 350, .y = 350}
#define P4_4P_COORDS Coord2D{.x = 500, .y = 350}
Coord2D getHpPopupBoxCoords(int playerNum) {
    SCENE_TYPE scene = getScene();
    char totalPlayers;

    // I don't know why training mode has the player numbers
    // backwards. :(
    if (scene == TRAINING_MODE_MMS) {
        totalPlayers = 4;
        switch(playerNum){
        case 0:
            return P2_4P_COORDS;
        case 1:
            return P1_4P_COORDS;
        default:
            return Coord2D{};
        }
    }

    totalPlayers = FIGHTER_MANAGER->getEntryCount();

    if (totalPlayers == 2) {
        switch(playerNum) {
            case 0:
                return P1_2P_COORDS;
            case 1:
                return P2_2P_COORDS;
            default:
                return Coord2D{};
        }
    }
    if (totalPlayers == 4) {
        switch(playerNum) {
            case 0:
                return P1_4P_COORDS;
            case 1:
                return P2_4P_COORDS;
            case 2:
                return P3_4P_COORDS;
            case 3:
                return P4_4P_COORDS;
            default:
                return Coord2D {};
        }
    }

    // TODO: Other numbers of players.

    return Coord2D{};
}

float fmax(float x1, float x2) {
    if (x1 > x2) return x1;
    return x2;
}

int max(int x1, int x2) {
    if (x1 > x2) return x1;
    return x2;
}

void drawAllPopups() {
    for(int i = 0; i < WISP_MAX_PLAYERS; i++) {
        auto itr = LinkedlistIterator(playerPopups[i]);
        Popup* popup;
        Coord2D coords = getHpPopupBoxCoords(i);


        while ((popup = itr.next()) != nullptr) {
            if (popup->expired(frameCounter)) {
                itr.deleteHere();
                delete popup;
            } else {
                popup->coords = coords;
                // OSReport("Set popup coords to %d,%d\n", coords.x, coords.y);
                popup->draw(printer, frameCounter);

                coords.y -= WISP_POPUP_VERTICAL_OFFSET;
            }
        }
    }
}