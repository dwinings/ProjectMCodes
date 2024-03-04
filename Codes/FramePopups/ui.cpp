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
    for (u32 i = 0; i < FIGHTER_MANAGER->getEntryCount(); i++) {
        auto& newPage = *(new Page(this));
        auto& player = allPlayerData[i];

        // OSReport("Adding page: 0x%x\n", (void*)newPage);
        snprintf(newPage.title, 256, "Fighter %d (%d)\n", i, player.charKind);
        newPage.addOption(new SpacerOption());
        newPage.addOption(new BoolOption("Popup: On-Shield Advantage", player.showOnShieldAdvantage));
        newPage.addOption(new BoolOption("Popup: On-Hit Advantage", player.showOnHitAdvantage));
        newPage.addOption(new SpacerOption());

        auto& playerInfoSubpage = *(new SubpageOption("Fighter Info", true));
        playerInfoSubpage.addOption(new PlayerDataShortHexObserver("Action ID", &player, &PlayerData::action));
        playerInfoSubpage.addOption(new PlayerDataStrObserver("Action", &player, &PlayerData::actionStr));
        playerInfoSubpage.addOption(new PlayerDataShortHexObserver("Subaction ID", &player, &PlayerData::subaction));
        playerInfoSubpage.addOption(new PlayerDataStrObserver("Subaction Name", &player, &PlayerData::subactionStr));
        playerInfoSubpage.addOption(new PlayerDataFlagObserver("RA Bits", &player, &PlayerData::raLowBits));
        newPage.addOption(&playerInfoSubpage);
        addPage(&newPage);
    }

    auto& displayOptsPage = *(new Page(this));
    snprintf(displayOptsPage.title, 256, "Display Options");
    displayOptsPage.addOption(new IntOption<int>("Menu X", pos.x));
    displayOptsPage.addOption(new IntOption<int>("Menu Y", pos.y));
    displayOptsPage.addOption(new IntOption<char>("Opacity", opacity));
    displayOptsPage.addOption(new FloatOption("Font Scaling", fontScaleMultiplier, (float)0, (float)100, (float)0.1));
    addPage(&displayOptsPage);

    currentPageIdx = 0;
    initialized = true;
}

void WispMenu::cleanup() {
    currentPageIdx = 0;
    pages.empty();
    initialized = false;
}

void WispMenu::handleInput() {
    auto& menu = *this;
    PADButtons btn;

    btn.bits = PREVIOUS_PADS[0].button.bits | PREVIOUS_PADS[1].button.bits | PREVIOUS_PADS[2].button.bits | PREVIOUS_PADS[3].button.bits;
    if (btn.bits == 0) {
        OSReport("Short circuit no buttons\n");
        return; // shortcut
    }

    if ((frameCounter - lastInputFrame) < WISP_MENU_INPUT_SPEED) {
        OSReport("Debounced input. Frame delta: %d", frameCounter - lastInputFrame);
        return; // debounce inputs.
    } else {
        menu.lastInputFrame = 0;
    }

    if (btn.L && btn.R && btn.UpDPad) {
        menu.toggle();
        return;
    }

    if (!menu.isActive()) {
        OSReport("Short circuit because inactive.\n");
        return; // reduce nesting.
    }

    if (btn.A) menu.select();   return;
    if (btn.B) menu.deselect(); return;
    if (menu.selected) {
        if      (btn.UpDPad   | btn.RightDPad) menu.modify(1);
        else if (btn.DownDPad | btn.LeftDPad)  menu.modify(-1);
        return;
    } else {
        if      (btn.UpDPad)    menu.up();
        else if (btn.DownDPad)  menu.down();
        else if (btn.LeftDPad)  menu.prevPage();
        else if (btn.RightDPad) menu.nextPage();
        return;
    }
}

float WispMenu::lineHeight() {
    return this->baseFontScale.y * fontScaleMultiplier * this->lineHeightMultiplier;
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
    printerMsgObj.fontScaleY = baseFontScale.y * fontScaleMultiplier;
    printerMsgObj.fontScaleX = baseFontScale.x * fontScaleMultiplier;
    printerMsgObj.xPos = pos.x;
    printerMsgObj.yPos = pos.y;
    printerMsgObj.zPos = 0;
    printer.start2D();

    printer.startBoundingBox();

    auto& currentPage = *getCurrentPage();
    snprintf(buffer, maxLen, "Page %d / %d: %s", currentPageIdx+1, pages.size(), currentPage.getTitle());
    printer.printLine(buffer);
    currentPage.render(&printer, buffer);

    printer.saveBoundingBox(printer.bboxIdx, COLOR_TRANSPARENT_GREY, COLOR_WHITE, 12, WISP_PRINTER_PADDING);
}