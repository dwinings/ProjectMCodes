#ifndef PROJECTMCODES_POPUP_H
#define PROJECTMCODES_POPUP_H

#include "common.h"
#include "Containers/vector.h"
#include "Graphics/Drawable.h"
#include "Graphics/TextPrinter.h"
#include "Brawl/Message.h"


class Popup {
    public:
        Popup(
            const char* text, 
            u32 startFrame
        );
        ~Popup();
        void addText(const char* bytes);
        void draw(TextPrinter& printer, u32 currentFrame);
        float percentElapsed(u32 currentFrame);
        bool expired(u32 currentFrame);

        // This allocates a new string!
        char* createCStr();

        u32 startFrame;
        u8 id = 0;
        u8 fps = 60;
        u16 durationSecs = 5;
        u16 minWidth = 100;
        GXColor color = 0xFFFFFFFF; // white
        Coord2D coords;
    private:
        Rect* progressRect;
        char* text;
};

void drawAllPopups(vector<Popup*>& popups, TextPrinter& printer, u32 currentFrame);

#endif