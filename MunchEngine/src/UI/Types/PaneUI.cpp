#include "UI/Types/PaneUI.h"

PaneUI::PaneUI(int posX, int posY, int width, int height, ParentType* parent)
    : UIElement(posX, posY, width, height, parent) {
}

void PaneUI::setup() {
    texture.create("Textures/floor.tga");
    mesh.createPlane();
    setupParams();
}