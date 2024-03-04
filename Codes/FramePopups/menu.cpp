// Wanton hoist of fudgepop's menu for my own purposes/modification

#include "menu.h"
#include "common.h"
#include <Graphics/Drawable.h>

#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

#pragma region Page
void Page::addOption(OptionType* option) {
    option->setParentPage(this);
    options.push(option);
}

void Page::deselect() {
    if (isSelected) {
        options[currentOption]->deselect();
        isSelected = options[currentOption]->isSelected;
    }
}

void Page::select() {
    options[currentOption]->select();
    isSelected = options[currentOption]->isSelected;
}

void Page::up() {
    if (isSelected) options[currentOption]->up();
    else if (currentOption > 0) {
        char start = currentOption;
        for (char i = --currentOption; i > 0 && !options[i]->canModify; i--) {
            currentOption--;
        }
        if (!options[currentOption]->canModify) currentOption = start;
    }
}
void Page::down() {
    if (isSelected) options[currentOption]->down();
    else if (currentOption < options.size() - 1) {
        char start = currentOption;
        char size = options.size();
        for (char i = ++currentOption; i < (size - 1) && !options[i]->canModify; i++) {
            currentOption++;
        }
        if (!options[currentOption]->canModify) currentOption = start;
    }
}

void Page::modify(float amount) {
    options[currentOption]->modify(amount);
}

void Page::render(TextPrinter* printer, char* buffer) {
    auto& m = *menu;
    char len = options.size();

    for (char i = 0; i < len; i++) {
        if (!options[i]->canModify) printer->setTextColor(applyAlpha(m.readOnlyColor, m.opacity));
        else if (i == currentOption && isSelected && menu->paused) {
            printer->setTextColor(applyAlpha(m.selectedColor, m.opacity));
        }
        else if (i == currentOption && menu->paused) {
            printer->setTextColor(applyAlpha(m.highlightedColor, m.opacity));
        }
        else {
            printer->setTextColor(applyAlpha(m.defaultColor, m.opacity));
        }
        printer->padToWidth(RENDER_X_SPACING / 5);
        options[i]->render(printer, buffer);
    }
}

void Page::hide() {}
void Page::show() {}

const char* Page::getTitle() { return this->title; }
#pragma endregion

#pragma region Menu
void Menu::addPage(Page* p) {
    p->menu = this;
    pages.push(p);
    if (currentPageIdx == -1) { currentPageIdx = 0; }
}

void Menu::nextPage() {
    currentPageIdx += 1;
    if (currentPageIdx == pages.size()) { currentPageIdx = 0; }
}

void Menu::prevPage() {
    currentPageIdx -= 1;
    if (currentPageIdx == -1) { currentPageIdx = pages.size() - 1; }
}

Page* Menu::getCurrentPage() {
    if (pages.size() == 0) { return nullptr; }
    return pages[currentPageIdx];
}

void Menu::select() { getCurrentPage()->select(); selected = getCurrentPage()->isSelected; }
void Menu::deselect() {
    getCurrentPage()->deselect(); selected = getCurrentPage()->isSelected;
}
void Menu::up() { getCurrentPage()->up(); }
void Menu::down() { getCurrentPage()->down(); }

void Menu::modify(float amount) { getCurrentPage()->modify(amount); }

void Menu::render(TextPrinter* printer, char* buffer) {
    printer->startBoundingBox();
    char internalBuf[128];
    snprintf(internalBuf, 128, "Page 1 / %d", pages.size());
    printer->printLine(buffer);
    printer->printLine("");
    getCurrentPage()->render(printer, buffer);
    printer->saveBoundingBox(0, 0x222222FF | (opacity & 0xFF), 0x000000FF | (opacity & 0xFF), 2, WISP_PRINTER_PADDING);
}
void Menu::unpause() { paused = false; }
void Menu::toggle() {
    if (visible || paused) {
        paused = false;
        visible = false;
    }
    else {
        paused = true;
        visible = true;
    }
}
#pragma endregion

#pragma region ModifiableScalars

void FloatOption::modify(float amount) {
    value += amount * changeMultiplier;
    if (max != NUMERIC_DEFAULT && value > max) value = min;
    else if (min != NUMERIC_DEFAULT && value < min) value = max;
}
void FloatOption::render(TextPrinter* printer, char* buffer) {
    sprintf(buffer, "%s: %.3f", name, value);
    printer->printLine(buffer);
}

void BoolOption::modify(float amount) {
    value = !value;
}
void BoolOption::render(TextPrinter* printer, char* buffer) {
    sprintf(buffer, "%s: %s", name, value ? "on" : "off");
    printer->printLine(buffer);
}
#pragma endregion

#pragma region ControlOption
void ControlOption::modify(float amount) {}
void ControlOption::select() {
    value = true;
    this->parent->menu->paused = false;
}
void ControlOption::deselect() {
    value = false;
    this->parent->menu->paused = true;
}
void ControlOption::render(TextPrinter* printer, char* buffer) {
    sprintf(buffer, "%s", name);
    printer->printLine(buffer);
}
#pragma endregion

#pragma region StringOption
void StringOption::modify(float amount) {}
void StringOption::render(TextPrinter* printer, char* buffer) {
    sprintf(buffer, "%s: %s", name, value);
    printer->printLine(buffer);
}
#pragma endregion


#pragma region NamedIndexOption
void NamedIndexOption::modify(float amount) {}
void NamedIndexOption::select() {}
void NamedIndexOption::deselect() {}
void NamedIndexOption::render(TextPrinter* printer, char* buffer) {
    sprintf(buffer, "%s: %s", name, (0 <= index && index < arrayLength) ? nameArray[index] : " ");
    printer->printLine(buffer);
}
#pragma endregion

#pragma region HexObserver
void HexObserver::modify(float amount) {}
void HexObserver::render(TextPrinter* printer, char* buffer) {
    switch (size) {
    case HexSize::CHAR:
        sprintf(buffer, "%s: 0x%02x", name, *value);
        break;
    case HexSize::SHORT:
        sprintf(buffer, "%s: 0x%04x", name, *value);
        break;
    case HexSize::INT:
        sprintf(buffer, "%s: 0x%08x", name, *value);
        break;
    }
    printer->printLine(buffer);
}
#pragma endregion

#pragma region SubpageOption
void SubpageOption::deselect() {
    /*
     * Either:
     *   - The subpage isn't selected, so nothing happens.
     *   - The subpage is selected + has a selected option, so deselect that option.
     *   - The subpage is selected and has no selected option, so deselect the subpage.
    */
    if (hasSelection) {
        options[currentOption]->deselect();
        if (this->subParent != nullptr) {
            this->subParent->hasSelection = true;
        }
        this->parent->isSelected = true;
    }
    else {
        isSelected = false;
    }
}

void SubpageOption::select() {
    /*
     * Either:
     *   - The subpage isn't selected, so select it.
     *   - The subpage is selected, so we select() the current option.
     *     - If that option is now selected, this option hasSelection == true
     *   - The subpage is selected, but we have nothing to select.
     *     - We toggle the selectedness of the subpage. Mashing A should fold/unfold collapsibles,
     *       for example.
     */

    if (isSelected) {
        // Has no changeable options.
        if (modifiableChildren == 0) {
            deselect();
        }
        else {
            options[currentOption]->select();
            hasSelection = options[currentOption]->isSelected;
        }
    }
    else {
        isSelected = true;
    }
}

void SubpageOption::modify(float amount) {
    if (options[currentOption]->canModify) {
        options[currentOption]->modify(amount);
    }
}

void SubpageOption::up() {
    if (hasSelection) options[currentOption]->up();
    else if (currentOption > 0) {
        char start = currentOption;
        for (char i = --currentOption; i > 0 && !options[i]->canModify; i--) {
            currentOption--;
        }
        if (!options[currentOption]->canModify) currentOption = start;
        if (currentOption < (scrollIdx + (height / 2))) scrollIdx--;
    }
}
void SubpageOption::down() {
    if (hasSelection) options[currentOption]->down();
    else if (currentOption < options.size() - 1) {
        char start = currentOption;
        char size = options.size();
        for (char i = ++currentOption; i < (size - 1) && !options[i]->canModify; i++) {
            currentOption++;
        }
        if (!options[currentOption]->canModify) currentOption = start;
        if (currentOption > (scrollIdx + (height / 2))) scrollIdx++;
    }
}

void SubpageOption::render(TextPrinter* printer, char* buffer) {
    int len = options.size();
    if (scrollIdx > (len - height)) scrollIdx = len - height;
    if (scrollIdx < 0) scrollIdx = 0;

    if (currentOption >= len) currentOption = len - 1;
    sprintf(buffer, (collapsible) ? ((isSelected) ? "v %s" : "> %s") : "%s:", name);
    printer->printLine(buffer);

    if (collapsible && isSelected || !(collapsible)) {
        for (int i = scrollIdx; i < (scrollIdx + height); i++) {
            if (i >= len) {
                printer->printLine("");
            }
            else {
                auto& m = *(parent->menu);

                if (!options[i]->canModify) {
                    printer->setTextColor(applyAlpha(m.readOnlyColor, m.opacity));
                }
                else if (i == currentOption && isSelected && m.paused) {
                    printer->setTextColor(applyAlpha(m.selectedColor, m.opacity));
                }
                else if (i == currentOption && m.paused) {
                    printer->setTextColor(applyAlpha(m.highlightedColor, m.opacity));
                }
                else {
                    printer->setTextColor(applyAlpha(m.defaultColor, m.opacity));
                }
                printer->padToWidth((RENDER_X_SPACING / 5) * (depth + 1));
                options[i]->render(printer, buffer);
            }
        }
    }
}

void SubpageOption::addOption(OptionType* option) {
    option->parent = this->parent;
    option->subParent = this;
    options.push(option);

    if (option->canModify) {
        modifiableChildren++;
    }
}

void SubpageOption::setParentPage(Page* p) {
    this->parent = p;
    int len = options.size();
    for (int i = 0; i < len; i++) options[i]->setParentPage(p);
}

void SubpageOption::clearOptions() {
    options.clear();
}

void SubpageOption::removeOptions() {
    options.reallocate(0);
    options.reallocate(1);
}

int SubpageOption::getOptionCount() {
    return options.size();
}

#pragma endregion