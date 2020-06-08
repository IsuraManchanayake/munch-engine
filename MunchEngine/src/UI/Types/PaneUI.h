#pragma once

#include "UI/UIElement.h"

struct PaneUI : UIElement {
    PaneUI(int posX, int posY, int width, int height, ParentType* parent);

    void setup() override;
};