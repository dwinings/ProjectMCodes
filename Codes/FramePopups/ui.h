#ifndef WISP_UI

#include "menu.h"
#include "common.h"
#include "linkedlist.h"
#include "popup.h"
#include "playerdata.h"

#include <Wii/PAD/PADButtons.h>
#include <Wii/PAD/PADStatus.h>

// struct Menu {
//   void nextPage(Page* p);
//   Page* getCurrentPage();
//   void select();
//   void deselect();
//   void up();
//   void down();
//   void modify(float amount);
//   void render(TextPrinter* printer, char* buffer);
//   void unpause();
//   void toggle();
//   bool visible = false;
//   bool paused = false;
//   bool selected = false;
//   vector<Page*> pages;
//   vector<Page*> path;
//   float opacity = 0xDD;
// };
// Lower is faster
#define WISP_MENU_INPUT_SPEED 10
class WispMenu : public Menu {
    public:
        WispMenu() {};
        Coord2D pos = {200, 50};
        Coord2D size = {300, 300};
        Coord2DF baseFontScale = {0.45, 0.7};

        float fontScaleMultiplier = 1;
        int lineHeightMultiplier = 20;
        u8 outlineWidth = 2;
        u32 initialized: 1 = false;

        GXColor bgColor = COLOR_TRANSPARENT_GREY;
        GXColor outlineColor = COLOR_WHITE;

        void init();
        void cleanup();
        void handleInput();
        void render(TextPrinter& printer, char* buffer, u32 maxLen);
        void drawBg(TextPrinter& printer);
        void drawOutline(TextPrinter& printer);
        void drawHighlightBox();

        float lineHeight();

        inline bool isActive() { return this->visible && this->paused; };
    private:
        u32 lastInputFrame;
};

#define BO_METH_PTR(T, V) T const* instance, V (T::*getter) () const
template <typename T, typename V>
class BaseObserver: public StandardOption {
    public:
        BaseObserver(const char* name, BO_METH_PTR(T, V)) : instance(instance), getter(getter){
            this->name = name;
            canModify = false;
        };
        virtual ~BaseObserver() {};
        virtual void modify(float amount) {};
        virtual void select() {};
        virtual void deselect() {};
        virtual void render(TextPrinter* printer, char* buffer) {};
    protected:
        V (T::*getter)() const;
        T const* instance;

};

class PlayerDataFlagObserver : public BaseObserver<PlayerData, u32> {
public:
    PlayerDataFlagObserver(const char* name, BO_METH_PTR(PlayerData, u32)) : BaseObserver(name, instance, getter) {};
    virtual void render(TextPrinter* printer, char* buffer) {
        auto byte = (*(this->instance).*getter)();
        char binBuf[50];
        char* bufPos = binBuf;
        char val;

        for (char i = 0; i < 32; i++) {
            val = (byte >> i) & 0x1;
            *bufPos++ = (val == 0 ? '0' : '1');
            if (i % 8 == 7) {
                *bufPos++ = ' ';
            }
        }
        *bufPos = '\0'; 

        sprintf(buffer, "%s: %s", name, binBuf);
        printer->printLine(buffer);
    };
};

class PlayerDataHexObserver : public BaseObserver<PlayerData, u32> {
public:
    PlayerDataHexObserver(const char* name, BO_METH_PTR(PlayerData, u32)) : BaseObserver(name, instance, getter) {};
    virtual void render(TextPrinter* printer, char* buffer) {
        auto value = (*(this->instance).*getter)();
        sprintf(buffer, "%s: 0x%08x", name, value);
        printer->printLine(buffer);
    };
};

class PlayerDataShortHexObserver : public BaseObserver<PlayerData, u16> {
public:
    PlayerDataShortHexObserver(const char* name, BO_METH_PTR(PlayerData, u16)) : BaseObserver(name, instance, getter) {};
    virtual void render(TextPrinter* printer, char* buffer) {
        auto value = (*(this->instance).*getter)();
        sprintf(buffer, "%s: 0x%04x", name, value);
        printer->printLine(buffer);
    };
};

class PlayerDataByteHexObserver : public BaseObserver<PlayerData, char> {
public:
    PlayerDataByteHexObserver(const char* name, BO_METH_PTR(PlayerData, char)) : BaseObserver(name, instance, getter) {};
    virtual void render(TextPrinter* printer, char* buffer) {
        auto value = (*(this->instance).*getter)();
        sprintf(buffer, "%s: 0x%02x", name, value);
        printer->printLine(buffer);
    };
};

class PlayerDataStrObserver: public BaseObserver<PlayerData, const char*> {
public:
    PlayerDataStrObserver(const char* name, BO_METH_PTR(PlayerData, const char*)) : BaseObserver(name, instance, getter) {};
    virtual void render(TextPrinter* printer, char* buffer) {
        auto value = (*(this->instance).*getter)();
        sprintf(buffer, "%s: %s", name, value);
        printer->printLine(buffer);
    };
};

extern linkedlist<Popup> playerPopups[WISP_MAX_PLAYERS];
extern WispMenu& globalWispMenu;

#endif
#define WISP_UI