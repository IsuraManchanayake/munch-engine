#include "UI/Types/PaneUI.h"
#include "UI/UILayer.h"
#include "Core/Logger.h"

#include <GLFW/glfw3.h>

PaneUI::PaneUI(int posX, int posY, int width, int height, ParentType* parent, UILayer* layer)
    : UIElement(posX, posY, width, height, parent, layer) {
}

void PaneUI::setup() {
    texture.create("Textures/floor.tga");
    mesh.createPlane();
    setupParams();
}

void PaneUI::mouseEnter() {
    Logger::warn(id, " mouse enter");
}

void PaneUI::mouseInside() {
    Logger::warn(id, " mouse inside");
    if(layer->mouse.button == GLFW_MOUSE_BUTTON_LEFT && layer->mouse.action == GLFW_PRESS) {
        if(layer->currentDraggingUIElementId == 0 || layer->currentDraggingUIElementId == id) {
            layer->currentDraggingUIElementId = id;
            move(layer->mouse.dx, layer->mouse.dy, layer->mouse.dx, layer->mouse.dy);
        }
    }
}

void PaneUI::mouseLeave() {
    Logger::warn(id, " mouse leave");
}

void PaneUI::mousePress() {
}
