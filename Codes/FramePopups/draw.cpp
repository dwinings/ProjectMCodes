
// to print text to the screen
#include <Graphics/TextPrinter.h>
// controller input stuff
#include <Wii/PAD/PADStatus.h>
#include "Assembly.h"
#include "Memory.h"
// responsible for sound effects
#include "Brawl/sndSystem.h"
#include "Brawl/FT/ftManager.h"
#include "Brawl/FT/ftSlotManager.h"

#include "Graphics/Drawable.h"
#include "./draw.h"
#include "./linkedlist.h"


// makes menu feel smooth
signed char timer = 5;
signed char cmdDelay = 50;
bool instantResponse = true;
// show menu / pause game execution
bool paused = false;
bool visible = false;
bool haveSaidHello = false;
int wispMenuXPos = 200;
int wispMenuYPos = 50;
u8 wispAlpha = 255;

char strManipBuffer[WISP_STR_MANIP_SIZE]; 
float wispFontScaleX = 0.45;
float wispFontScaleY = 0.7;
int wispLineHeightMultiplier = 20;
unsigned int frameCounter = 0;
unsigned int popupCounter = 0;
signed char p1Timer;

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
    if (btn.L && btn.R && btn.DownDPad) {
        wispAlpha = max(0, wispAlpha - 1);
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

            /*
            if (allPlayerData[i].didEnableCancel()) {
                snprintf(strManipBuffer, WISP_STR_MANIP_SIZE, "IASA Frame %d/%d\n", 
                    allPlayerData[i].current->currentFrame, allPlayerData[i].current->totalFrames
                );
                Popup& p = *(new Popup(strManipBuffer, frameCounter));
                p.durationSecs = 3;
                p.minWidth = 150;
                p.coords = getHpPopupBoxCoords(i); 
                playerPopups[i].append(p);
            }
            */
        }

        for (player = 0; player < fighterCount; player++) {
            resolveAttackTarget(player);
        }

        for (player = 0; player < fighterCount; player++) {
            checkAttackTargetActionable(player);
        }

        // OSReport("StrManipBuffer: 0x%X\n", strManipBuffer);
        /*
        auto player0 = allPlayerData[0];
        player0.debugStr(strManipBuffer);
        printMessage(strManipBuffer, wispMenuXPos, wispMenuYPos);
        */

        /*
        if (player0.didEnterShield()) {
            Popup& p = *(new Popup("Hey, player 1 entered shield.\n", frameCounter));
            p.durationSecs = 3;
            p.minWidth = 150;
            playerPopups[0].append(p);
        }
        */


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

    /* hitstun/shieldstun stuff comes from the work module. */
    if (workModule != nullptr) {
        // OSReport("Work module vals for player %d:\n", player);
        // debugWorkModule(*workModule);
        auto RABasicsArr = (*(int(*)[workModule->RAVariables->basicsSize])workModule->RAVariables->basics);
        auto RAFloatArr = (*(int(*)[workModule->RAVariables->floatsSize])workModule->RAVariables->floats);
        auto LABasicsArr = (*(int(*)[workModule->LAVariables->basicsSize])workModule->LAVariables->basics);
        auto LAFloatArr = (*(float(*)[workModule->LAVariables->floatsSize])workModule->LAVariables->floats);


        // float shieldValue = LAFloatArr[0x3];

        currentData.shieldstun = RABasicsArr[0x5];
        if (currentData.shieldstun != 0) {
            if (currentData.shieldstun != (prevData.shieldstun - 1)) {
                playerData.maxShieldstun = RABasicsArr[0x5];
            }
            if (currentData.shieldstun == 0) {
                playerData.maxShieldstun = 0;
            }
        }

        int remainingHitstun = LABasicsArr[0x38];
        playerData.current->hitstun = remainingHitstun;
        if (playerData.didReceiveHitstun()) {
            playerData.maxHitstun = remainingHitstun;
        }
    }

    if (statusModule != nullptr) {
        /* OSReport("Action number: %x\n", statusModule->action); */
        // never seems to work. strcpy(playerData.current->actionname, statusModule->getStatusName(), WISP_ACTION_NAME_LEN);
        playerData.current->action = statusModule->action;
        if (statusModule->attackHasConnected) {
            playerData.didConnectAttack = true;
        }
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
        currentData.canCancel = (bool)isEnableCancel;
    }
}

void resolveAttackTarget(u8 playerNum) {
    PlayerData& player = allPlayerData[playerNum];
    // False most of the time, so this isn't as slow as it looks.
    if (player.didConnectAttack != false && player.attackTarget == nullptr) {
        for (char otherP = 0; otherP < WISP_MAX_PLAYERS; otherP++) {
            if (playerNum == otherP) {
                continue;
            }

            if (allPlayerData[otherP].didReceiveHitstun()){
                player.attackTarget = &(allPlayerData[otherP]);
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
            !isAttackingAction(player.current->action)
            || player.current->canCancel
        );

        bool targetIsActionable = (
            target.current->hitstun == 0 && target.current->shieldstun == 0
            && !startsWith(target.current->subactionName, "Landing")
        );

        if (playerIsActionable && player.becameActionableOnFrame == -1) {
            player.becameActionableOnFrame = frameCounter;
        }

        if (targetIsActionable && target.becameActionableOnFrame == -1) {
            target.becameActionableOnFrame = frameCounter;
        }

        if (playerIsActionable && targetIsActionable) {
            /* POP UP TIME! */
            short int advantage = player.becameActionableOnFrame - target.becameActionableOnFrame;

            snprintf(strManipBuffer, WISP_STR_MANIP_SIZE, "Advantage: %d\n", advantage);
            Popup& popup = *(new Popup(strManipBuffer, frameCounter));
            popup.coords = getHpPopupBoxCoords(playerNum);
            popup.durationSecs = 3;
            playerPopups[playerNum].append(popup);

            player.attackTarget = nullptr;
            player.becameActionableOnFrame = -1;
            player.didConnectAttack = false;
            target.becameActionableOnFrame = -1;
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

Coord2D P1_2P_COORDS = Coord2D{.x = 200, .y = 350};
Coord2D P2_2P_COORDS = Coord2D{.x = 355, .y = 350};
Coord2D getHpPopupBoxCoords(int playerNum) {
    int totalPlayers = FIGHTER_MANAGER->getEntryCount();
    if (totalPlayers == 2) {
        if (playerNum == 0) {
            return P1_2P_COORDS;
        }

        if (playerNum == 1) {
            return P2_2P_COORDS;
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

GXColor textColor() {
    GXColor color;
    color.red = 255;
    color.blue = 255;
    color.green = 255;
    color.alpha = wispAlpha;
    return color;
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