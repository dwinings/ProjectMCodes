#ifndef WISP_UI

#include "./common.h"
#include "./linkedlist.h"
#include "./popup.h"

#include <Wii/PAD/PADButtons.h>
#include <Wii/PAD/PADStatus.h>

class WispMenu {
    public:
        u32 gamePaused: 1 = false;
        u32 visible: 1 = false;

        Coord2D pos = {200, 50};
        u8 alpha = 255;

        Coord2DF fontScale = {0.45, 0.7};
        int lineHeightMultiplier = 20;

        void handleInput();
        float lineHeight();
};

extern linkedlist<Popup> playerPopups[WISP_MAX_PLAYERS];
extern WispMenu globalWispMenu;

#endif
#define WISP_UI