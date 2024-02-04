
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
#include "./menu.h"
#include "./linkedlist.h"


// setup global variables
Menu* myMenu;
// makes menu feel smooth
signed char timer = 5;
signed char cmdDelay = 50;
bool instantResponse = true;
// show menu / pause game execution
bool paused = false;
bool visible = false;
bool haveSaidHello = false;
int wispMenuXPos = 200;
int wispMenuYPos = 350;
u8 wispAlpha = 255;

char strManipBuffer[200] = {}; 
float wispFontScaleX = 0.45;
float wispFontScaleY = 0.7;
int wispLineHeightMultiplier = 20;
unsigned int frameCounter = 0;
unsigned int popupCounter = 0;
signed char p1Timer;

struct PlayerData allPlayerData[4] = {};
linkedlist<Popup> popups = linkedlist<Popup>();


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


void printMessage(char const* msg, GXColor color = COLOR_WHITE){
    OSReport(msg);
    printer.setup();
    printer.setTextColor(textColor());
    printer.renderPre = true;
    Message* printerMsgObj = &(printer.message);
    printerMsgObj->fontScaleY = wispFontScaleY;
    printerMsgObj->fontScaleX = wispFontScaleX;
    printer.lineHeight = printerMsgObj->fontScaleY * wispLineHeightMultiplier;
    printerMsgObj->xPos = wispMenuXPos;
    printerMsgObj->yPos = wispMenuYPos;
    printerMsgObj->zPos = 0;
    printer.setTextColor(color);
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

void swapPlayerDataFrames(u8 player) {
    PlayerDataOnFrame tmp = allPlayerData[player].current;
    allPlayerData[player].current = allPlayerData[player].prev;
    allPlayerData[player].prev = tmp;
    memset(&(allPlayerData[player].current), 0, sizeof(struct PlayerDataOnFrame));
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
    frameCounter += 1;
    char buffer[200] = {}; 
    renderables.renderPre();


    if (sceneType == SCENE_TYPE::VS || sceneType == SCENE_TYPE::TRAINING_MODE_MMS) {

        if (!haveSaidHello) {
            haveSaidHello = true;
            OSReport("Hello from wisp\n");
        }

        // Lazy debounce.
        if (frameCounter % 3 == 0) {
            handleInput();
        }

        /* Mysterious setup.
        printer.setup();

        printer.start2D();
        printer.setup();
        */

        Message* printerMsgObj = &(printer.message);

        OSReport("Message: 0x%08x\n", (void*)printerMsgObj);

        printer.setTextColor(textColor());
        printer.renderPre = true;
        printerMsgObj->fontScaleY = wispFontScaleY;
        printerMsgObj->fontScaleX = wispFontScaleX;
        printer.lineHeight = printerMsgObj->fontScaleY * wispLineHeightMultiplier;
        printerMsgObj->xPos = wispMenuXPos;
        printerMsgObj->yPos = wispMenuYPos;
        printerMsgObj->zPos = 0;


        int fighterCount;
        fighterCount = FIGHTER_MANAGER->getEntryCount();
        for (char i = 0; i < fighterCount; i++) {
            gatherData(i);
        }

        if (allPlayerData[0].didEnterShield()) {
            Coord2D* coords = getHpPopupBoxCoords(1, 2);
            char* popupBuffer = new char[256];
            sprintf(popupBuffer, "Hey, player 1 entered shield.", popupCounter);
            Popup* p = new Popup(popupBuffer, frameCounter);
            p->xPos = coords->x;
            p->yPos = coords->y;
            p->durationSecs = 3;
            p->minWidth = 100;
            popups.append(*p);
        }
    }


    drawAllPopups();
    startNormalDraw();
}


void gatherData(u8 player) {
    if (player > 3) {
        OSReport("Asked to gather data for invalid player %d\n", player);
        return;
    }

    swapPlayerDataFrames(player);

    PlayerData& playerData = allPlayerData[player];
    EntryID entryId = FIGHTER_MANAGER->getEntryId(player);
    Fighter* fighter = FIGHTER_MANAGER->getFighter(entryId, 0);
    int character = fighter->getFtKind();

    auto workModule = fighter->modules->workModule;
    auto statusModule = fighter->modules->statusModule;
    auto motionModule = fighter->modules->motionModule;

    /* hitstun/shieldstun stuff comes from the work module. */
    if (workModule != nullptr) {
        auto RABasicsArr = (*(int(*)[workModule->RAVariables->basicsSize])workModule->RAVariables->basics);
        auto LAFloatArr = (*(float(*)[workModule->LAVariables->floatsSize])workModule->LAVariables->floats);
        float shieldValue = LAFloatArr[0x3];

        float prevFrameShieldstun = 0; //fixme
        float shieldstun = RABasicsArr[0x5];
        if (shieldstun != prevFrameShieldstun - 1 && shieldstun != 0) {
            float maxShieldstun = RABasicsArr[0x5];
        }

        auto LABasicsArr = (*(int(*)[workModule->LAVariables->basicsSize])workModule->LAVariables->basics);
        int remainingHitstun = LABasicsArr[56];

        playerData.current.hitstun = remainingHitstun;
        if (playerData.didReceiveHitstun()) {
            playerData.maxHitstun = remainingHitstun;
        }
    }

    if (statusModule != nullptr) {
        statusModule->getStatusName1(statusModule->action);
    }



    soAnimChr animationData = motionModule->mainAnimationData;
    /* subaction stuff */
    if (animationData.resPtr != nullptr) {
        auto animationResource = animationData.resPtr->CHR0Ptr;
        if (animationResource == nullptr) {
            strcpy(playerData.current.subactionName, "UNKNOWN", WISP_ACTION_NAME_LEN);
            playerData.current.totalFrames = -1;
        }

        playerData.current.currentFrame = (u32)animationData.animFrame;
        strcpy(playerData.current.subactionName, animationResource->getString(), WISP_ACTION_NAME_LEN);
        playerData.current.totalFrames = animationResource->animLength;
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

Coord2D P1_2P_COORDS = (Coord2D){.x = 200, .y = 350};
Coord2D P2_2P_COORDS = (Coord2D){.x = 355, .y = 350};
Coord2D* getHpPopupBoxCoords(int playerNum, int totalPlayers) {
    if (totalPlayers == 2) {
        if (playerNum == 1) {
            return &P1_2P_COORDS;
        }

        if (playerNum == 2) {
            return &P2_2P_COORDS;
        }
    }

    // TODO: Other numbers of players.

    return nullptr;
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
    auto itr = popups.iterator();
    Popup* popup;

    while ((popup = itr->next()) != nullptr) {
        if (popup->expired(frameCounter)) {
            itr->deleteHere();
            delete popup;
        } else {
            popup->draw(printer, frameCounter);
        }
    }

    delete itr;
}