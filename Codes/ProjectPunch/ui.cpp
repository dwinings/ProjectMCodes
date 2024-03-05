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
    padding = 25;
    bgColor = 0x2D2D2DDD;  // dark grey
    highlightedColor = 0xEEE8AAFF; //ylw
    selectedColor = 0xB0C4DEFF; // blu
    defaultColor = 0xFFFFFFFF; 
    outlineColor = 0xBBBBBBFF; // light grey
    highlightBoxColor = 0x000000FF;

    for (u32 i = 0; i < FIGHTER_MANAGER->getEntryCount(); i++) {
        auto& newPage = *(new Page(this));
        auto& player = allPlayerData[i];

        // OSReport("Adding page: 0x%x\n", (void*)newPage);
        snprintf(newPage.title, 256, "Fighter %d (%d)", i, player.charKind);
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
    displayOptsPage.addOption(new IntOption<u8>("Opacity", opacity, 65, 255, true, true));
    displayOptsPage.addOption(new FloatOption("Title Font Scaling", titleFontScaleMultiplier, 0.0F, 10.0F, 0.1F));
    displayOptsPage.addOption(new FloatOption("Font Scaling", fontScaleMultiplier, 0.0F, 10.0F, 0.1F));

    auto& menuSizeSubPage = *(new SubpageOption("Menu Position", true));
    menuSizeSubPage.addOption(new IntOption<int>("Menu X", pos.x));
    menuSizeSubPage.addOption(new IntOption<int>("Menu Y", pos.y));
    menuSizeSubPage.addOption(new IntOption<int>("Menu W", size.x));
    menuSizeSubPage.addOption(new IntOption<int>("Menu H", size.y));
    displayOptsPage.addOption(&menuSizeSubPage);

    auto& bgColorSubP = *(new SubpageOption("Menu BG", true));
    bgColorSubP.addOption(new IntOption<u8>("R", bgColor.red, 0, 255, true, true));
    bgColorSubP.addOption(new IntOption<u8>("G", bgColor.blue, 0, 255, true, true));
    bgColorSubP.addOption(new IntOption<u8>("B", bgColor.green, 0, 255, true, true));
    bgColorSubP.addOption(new IntOption<u8>("A", bgColor.alpha, 0, 255, true, true));
    displayOptsPage.addOption(&bgColorSubP);
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

    btn.bits = (
        PREVIOUS_PADS[0].button.bits 
        | PREVIOUS_PADS[1].button.bits 
        | PREVIOUS_PADS[2].button.bits 
        | PREVIOUS_PADS[3].button.bits
    ) & 0b0001111101111111; // mask out the unknown button fields.
    if (btn.bits == 0) {
        return; // shortcut
    }

    if (!btn.X && ((frameCounter - lastInputFrame) < WISP_MENU_INPUT_SPEED)) {
        return; // debounce inputs.
    } 

    lastInputFrame = frameCounter;


    if (btn.L && btn.R && btn.UpDPad) {
        menu.toggle();
        return;
    }

    if (!menu.isActive()) {
        return; // reduce nesting.
    }

    if (btn.A) { menu.select();   return; }
    if (btn.B) { menu.deselect(); return; }
    /*
    if (menu.selected) {
        if      (btn.UpDPad   | btn.RightDPad) menu.modify(btn.Y ?  5 :  1);
        else if (btn.DownDPad | btn.LeftDPad)  menu.modify(btn.Y ? -5 : -1);
        return;
    } else {
        */
        if      (btn.UpDPad)    menu.up();
        else if (btn.DownDPad)  menu.down();
        else if (btn.RightDPad) menu.modify(btn.Y ?  5 :  1);
        else if (btn.LeftDPad)  menu.modify(btn.Y ? -5 : -1);
        else if (btn.L)  menu.prevPage();
        else if (btn.R)  menu.nextPage();
        else if (btn.Start) menu.unpause();
        return;
    //}
}

float WispMenu::lineHeight() {
    return this->baseFontScale.y * fontScaleMultiplier * this->lineHeightMultiplier;
}

void WispMenu::render(TextPrinter& printer, char* buffer, u32 maxLen) {
    if (!visible) { return; }
    if (pages.size() == 0) {
        OSReport("Tried to render a menu with no pages.\n");
        return;
    }

    highlightedOptionBottom = 0;
    highlightedOptionTop = 0;

    /* Setup */
    printer.setup();
    printer.renderPre = true;
    Message& printerMsgObj = printer.message;
    printerMsgObj.xPos = pos.x + padding;
    printerMsgObj.yPos = pos.y + padding;
    printerMsgObj.zPos = 0;
    printer.start2D();

    printer.startBoundingBox();

    /* Print title */
    auto& currentPage = *getCurrentPage();
    snprintf(buffer, maxLen, "Page %d / %d: %s", currentPageIdx+1, pages.size(), currentPage.getTitle());
    printerMsgObj.fontScaleY = baseFontScale.y * titleFontScaleMultiplier;
    printerMsgObj.fontScaleX = baseFontScale.x * titleFontScaleMultiplier;
    printer.lineHeight = this->baseFontScale.y * titleFontScaleMultiplier * this->lineHeightMultiplier;
    printer.setTextColor(applyAlpha(highlightedColor, opacity));
    printer.printLine(buffer);
    
    /* Print body */
    printerMsgObj.fontScaleY = baseFontScale.y * fontScaleMultiplier;
    printerMsgObj.fontScaleX = baseFontScale.x * fontScaleMultiplier;
    printer.lineHeight = lineHeight();
    printerMsgObj.yPos += padding;
    currentPage.render(&printer, buffer);

    /* Draw graphics */
    drawBg(printer);
    drawOutline(printer);
    drawHighlightBox();

    /* Reset */
    printer.setup();
}

void WispMenu::drawBg(TextPrinter& printer) {
    auto& message = printer.message;
    // void Te    
    renderables.items.preFrame.push(new Rect{
            0,
            1,
            applyAlpha(bgColor, opacity),
            (float)pos.y,
            //message.yPos + printer.lineHeight + padding,
            (float)pos.y + size.y,
            (float)pos.x,
            (float)(pos.x + size.x),
            true
    });
}

void WispMenu::drawOutline(TextPrinter& printer) {
    auto& message = printer.message;

    renderables.items.preFrame.push(new RectOutline{
            0,
            1,
            applyAlpha(outlineColor, opacity),
            (float)(pos.y - 1),
//            message.yPos + printer.lineHeight + padding + 1,
            (float)pos.y + size.y,
            (float)(pos.x - 1),
            (float)(pos.x + size.x + 1),
            outlineWidth * 6,
            true
    });
}

void WispMenu::drawHighlightBox() {
    if (highlightedOptionBottom == 0 || highlightedOptionTop == 0) { return; }

    renderables.items.preFrame.push(new Rect {
        0,
        1,
        applyAlpha(highlightBoxColor, opacity),
        highlightedOptionTop,
        highlightedOptionBottom,
        pos.x + padding,
        pos.x + size.x - padding,
        true
    });
}