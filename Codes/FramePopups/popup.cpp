#include "popup.h"

Popup::Popup(const char* text, u32 startFrame) {
    this->startFrame = startFrame;
    this->text = new char[strlen(text)];
    strcpy(this->text, text);
}

Popup::~Popup() {
    delete[] this->text;
}

void Popup::draw(TextPrinter& printer, u32 currentFrame) {
    // This will mess with the printer settings, you will likely have to reset draw mode and
    // so on after this is called.
    if (!this->expired(currentFrame)) {
        printer.setup();
        printer.setTextColor(COLOR_WHITE);
        printer.renderPre = true;
        Message* printerMsgObj = &(printer.message);
        printerMsgObj->fontScaleX = WISP_DEFAULT_FONT_SCALE_X;
        printerMsgObj->fontScaleY = WISP_DEFAULT_FONT_SCALE_Y;
        printer.lineHeight = printerMsgObj->fontScaleY * 20;
        printerMsgObj->xPos = coords.x;
        printerMsgObj->yPos = coords.y;
        printerMsgObj->zPos = 0;
        printer.start2D();

        printer.startBoundingBox();
        printer.print(this->text);
        printer.padToWidth(this->minWidth);


        // Gradient from yellow to red based on progress.
        GXColor progressColor = 0;
        progressColor.red = 255;
        float elapsed = this->percentElapsed(currentFrame);
        progressColor.green = ((1 - elapsed) * 255);
        progressColor.blue = 0;
        progressColor.alpha = 255;
        float actualWidth;


        if (printer.lineStart + printer.maxWidth < printerMsgObj->xPos) {
            actualWidth = printerMsgObj->xPos - printer.lineStart;
        } else {
            actualWidth = printer.maxWidth;
        }


        int multiplier = (printer.is2D) ? 1 : -1;
        float left = printer.lineStart - WISP_PRINTER_PADDING;
        float right = printer.lineStart + actualWidth + WISP_PRINTER_PADDING;
        float lrdelta = right - left;

        // We can't just re-use this rect because the renderables vector implementation
        // will delete it after it has been drawn :(
        Rect* progressRect = new Rect {
            0,
            1,
            progressColor,
            ((printerMsgObj->yPos + printer.lineHeight + WISP_PRINTER_PADDING) * multiplier) - (7*multiplier),
            (printerMsgObj->yPos + printer.lineHeight + WISP_PRINTER_PADDING) * multiplier,
            left,
            right - (this->percentElapsed(currentFrame)*lrdelta),
            printer.is2D
        };

        #ifdef WISP_DEBUG
        OSReport("Rendering popup: %s", this->message);
        #endif
        renderables.items.preFrame.insert(progressRect, printer.bboxIdx+1);
        printer.saveBoundingBox(printer.bboxIdx, COLOR_TRANSPARENT_GREY, WISP_PRINTER_PADDING);
    }
}

float Popup::percentElapsed(u32 currentFrame) {
    u32 framesElapsed = currentFrame - this->startFrame;
    return framesElapsed / (float)(this->durationSecs * this->fps);
}

bool Popup::expired(u32 currentFrame) {
    return (this->percentElapsed(currentFrame) >= 1);
}
