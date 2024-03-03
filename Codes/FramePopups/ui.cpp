#include "./ui.h"

#include "./common.h"
#include "./linkedlist.h"
#include "./popup.h"
#include "./menu.h"
#include "./playerdata.h"

#include <Brawl/FT/ftManager.h>

extern u32 frameCounter;
extern PlayerData allPlayerData[WISP_MAX_PLAYERS];

/* EXTERN DEFS */
WispMenu& globalWispMenu = *(new WispMenu());
linkedlist<Popup> playerPopups[WISP_MAX_PLAYERS] = {
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>(),
    linkedlist<Popup>()
};
/************/

void WispMenu::init() {
    OSReport("Hello there.\n");
    for (u32 i = 0; i < FIGHTER_MANAGER->getEntryCount(); i++) {
        auto& newPage = *(new Page(this));
        auto& player = allPlayerData[i];

        // OSReport("Adding page: 0x%x\n", (void*)newPage);
        snprintf(newPage.title, 256, "Fighter %d (%d)", i, player.charKind);
        newPage.addOption(new BoolOption("Popup: On-Shield Advantage", player.showOnShieldAdvantage));
        newPage.addOption(new BoolOption("Popup: On-Hit Advantage", player.showOnHitAdvantage));
        newPage.addOption(new PlayerDataShortHexObserver("Action ID", &player, &PlayerData::action));
        newPage.addOption(new PlayerDataStrObserver("Action", &player, &PlayerData::actionStr));
        newPage.addOption(new PlayerDataShortHexObserver("Subaction ID", &player, &PlayerData::subaction));
        newPage.addOption(new PlayerDataStrObserver("Subaction Name", &player, &PlayerData::subactionStr));
        newPage.addOption(new PlayerDataFlagObserver("RA Bits", &player, &PlayerData::raLowBits));
        addPage(&newPage);

        currentPageIdx = 0;
        // OSReport("  Page options: 0x%x\n", (void*)&(newPage->options));
        // OSReport("  Page options size: %d\n", newPage->options.size());
    }


    initialized = true;
}

void WispMenu::handleInput() {
    auto& menu = *this;
    PADButtons btn;

    if (frameCounter - this->lastInputFrame < WISP_MENU_INPUT_SPEED) {
        return; // debounce inputs.
    }

    btn.bits = PREVIOUS_PADS[0].button.bits | PREVIOUS_PADS[1].button.bits | PREVIOUS_PADS[2].button.bits | PREVIOUS_PADS[3].button.bits;
    if (btn.bits == 0) {
        return; // shortcut
    }

    if (btn.L && btn.R && btn.UpDPad) {
        menu.toggle();
        menu.lastInputFrame = frameCounter;
        return;
    }

    if (!menu.isActive()) {
        return; // reduce nesting.
    }

    if (btn.B) {
        if (menu.selected) {
            menu.deselect();
        } else {
            menu.toggle();
        }
        return;
    }
    // menu is active now

    if (menu.selected) {
        if (btn.A | btn.B) {
            menu.deselect();
            return;
        }

        if (btn.UpDPad | btn.RightDPad) {
            menu.modify(1);
            return;
        }

        if (btn.DownDPad | btn.LeftDPad) {
            menu.modify(-1);
            return;
        }

        return;
    }

    // no option selected now.

    if (btn.UpDPad) {
        menu.up();
    }

    if (btn.DownDPad) {
        menu.down();
    }

    if (btn.LeftDPad) {
        menu.prevPage();
    }

    if (btn.RightDPad) {
        menu.nextPage();
    }
}

float WispMenu::lineHeight() {
    return this->fontScale.y * this->lineHeightMultiplier;
}

void WispMenu::render(TextPrinter& printer, char* buffer, u32 maxLen) {
    //if (!visible) { return; }
    if (pages.size() == 0) {
        OSReport("Tried to render a menu with no pages.\n");
        return;
    }
    printer.setup();
    printer.setTextColor(COLOR_WHITE);
    printer.renderPre = true;
    Message& printerMsgObj = printer.message;
    printer.lineHeight = lineHeight();
    printerMsgObj.fontScaleY = fontScale.y;
    printerMsgObj.fontScaleX = fontScale.x;
    printerMsgObj.xPos = pos.x;
    printerMsgObj.yPos = pos.y;
    printerMsgObj.zPos = 0;
    printer.start2D();

    printer.startBoundingBox();

    auto& currentPage = *getCurrentPage();
    snprintf(buffer, maxLen, "Page %d / %d: %s", currentPageIdx+1, pages.size(), currentPage.getTitle());
    printer.printLine(buffer);
    currentPage.render(&printer, buffer);

    printer.saveBoundingBox(printer.bboxIdx, COLOR_TRANSPARENT_GREY, WISP_PRINTER_PADDING);
}