#include "UI/UIRoot.h"

UIRoot::UIRoot(int posX, int posY, int width, int height, UILayer* layer)
    : AbstractUI<UIElement>(posX, posY, width, height, layer), modelMat(1.f) {
}

UIRoot::~UIRoot() {
}

void UIRoot::setup() {
    calcModel();
}

void UIRoot::render(GLint modelLocation) {
    for(auto& child : children) {
        child->render(modelMat, modelLocation);
    }
}

void UIRoot::calcModel() {
    modelMat = glm::translate(glm::mat4{1.f}, {-1.f, 1.f, 0.f});
    modelMat = glm::scale(modelMat, {2.f / width, -2.f / height, 1.f});
}

void UIRoot::mouseEnter() {
}

void UIRoot::mouseInside() {
}

void UIRoot::mouseLeave() {
}

void UIRoot::mousePress() {
}

void UIRoot::handleMouseMove(MouseMoveEvent& event) {
    for(auto& child : children) {
        child->handleMouseMove(event);
    }
}

void UIRoot::handleMousePress(MousePressEvent& event) {
    for(auto& child : children) {
        child->handleMousePress(event);
    }
}
