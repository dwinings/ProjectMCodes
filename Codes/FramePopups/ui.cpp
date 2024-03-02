#include "./ui.h"

/* EXTERNS */
WispMenu globalWispMenu = WispMenu();
linkedlist<Popup> playerPopups[WISP_MAX_PLAYERS] = {
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>()
};
/************/


void WispMenu::handleInput() {
    if (frameCounter % 3 != 0) {
        return; // poor man's debounce
    }

    auto& menu = *this;

    PADButtons btn;
    btn.bits = PREVIOUS_PADS[0].button.bits | PREVIOUS_PADS[1].button.bits | PREVIOUS_PADS[2].button.bits | PREVIOUS_PADS[3].button.bits;


    if (btn.L && btn.R && btn.UpDPad) {
        menu.alpha += 1;
        return;
    }

    if (btn.L && btn.LeftDPad) {
        menu.pos.x = max(0, menu.pos.x - 1);
        return;
    }
    if (btn.L && btn.RightDPad) {
        menu.pos.x = menu.pos.x + 1;
        return;
    }
    if (btn.L && btn.DownDPad) {
        menu.pos.y = max(0, menu.pos.y - 1);
        return;
    }
    if (btn.L && btn.UpDPad) {
        menu.pos.y += 1;
        return;
    }

    if (btn.R && btn.LeftDPad) {
        menu.fontScale.x = fmax(0, menu.fontScale.x - WISP_FONT_SCALE_INC);
        return;
    }
    if (btn.R && btn.RightDPad) {
        menu.fontScale.x += WISP_FONT_SCALE_INC;
        return;
    }
    if (btn.R && btn.DownDPad) {
        menu.fontScale.y = fmax(0, menu.fontScale.y - WISP_FONT_SCALE_INC);
        return;
    }
    if (btn.R && btn.UpDPad) {
        menu.fontScale.y += WISP_FONT_SCALE_INC;
        return;
    }
    if (btn.R && btn.A) {
        menu.lineHeightMultiplier += 1;
        return;
    }
    if (btn.R && btn.B) {
        menu.lineHeightMultiplier = max(0, menu.lineHeightMultiplier - 1);
        return;
    }
}

float WispMenu::lineHeight() {
    return this->fontScale.y * this->lineHeightMultiplier;
}