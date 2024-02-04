#ifndef WISP_DRAW_H
#define WISP_DRAW_H

#include "stddef.h"
#include "Brawl/SO/SoMotionModule.h"
#include "CLibs/cstring.h"
#include "Graphics/Drawable.h"

#include "common.h"
#include "playerdata.h"
#include "popup.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)

//hacky way to check if in game
enum SCENE_TYPE {
    MAIN_MENU = 0x1,
    HRC_CSS = 0x2,
    DIFFICULTY_TRAINING_CSS = 0x4,
    CSS_HRC = 0x5,
    DIFFICULTY_TRAINING_SSS_EVT_CSS = 0x6,
    SSS_BTT = 0x7,
    TRAINING_MODE_MMS = 0x8,
    VS = 0xA,
    UNKNOWN=0xFF
};


SCENE_TYPE getScene();

Coord2D getHpPopupBoxCoords(int playerNum);

GXColor textColor();
float fmax(float x1, float x2);
int max(int x1, int x2);

void gatherData(u8 fighter);
void drawAllPopups();



/* Static value defines */

#define COLOR_TRANSPARENT_GREY ((GXColor)0x000000DD)
#define COLOR_WHITE ((GXColor)0xFFFFFFEE)
#define WISP_ACTION_NAME_LEN 32
#define WISP_PRINTER_PADDING 5
#define WISP_FONT_SCALE_INC 0.1
#define WISP_DEFAULT_MSG_X_POS 100
#define WISP_DEFAULT_MSG_Y_POS 100
#define WISP_DEFAULT_FONT_SCALE_X 0.5
#define WISP_DEFAULT_FONT_SCALE_Y 0.7
#define WISP_POPUP_VERTICAL_OFFSET 75
#define WISP_MAX_PLAYERS 4

#endif