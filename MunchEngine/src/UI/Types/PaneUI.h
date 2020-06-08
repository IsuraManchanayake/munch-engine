#pragma once

#include "UI/UIElement.h"

struct PaneUI : UIElement {
    PaneUI(int posX, int posY, int width, int height, ParentType* parent, UILayer* layer);
    
    void mouseEnter() override;
    void mouseInside() override;
    void mouseLeave() override;
    void mousePress() override;

    void setup() override;
};