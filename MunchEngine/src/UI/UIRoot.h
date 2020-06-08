#pragma once

#include "UI/AbstractUI.h"
#include "UI/UIElement.h"

struct UIRoot final : AbstractUI<UIElement> {
    UIRoot(int posX, int posY, int width, int height);
    ~UIRoot();

    void setup() override;
    void render(GLint modelLocation);
    template<typename UIElementType, typename... Args>
    UIElementType* addUIElement(Args&&... args);
    void calcModel();
    void mouseEnter() override;
    void mouseInside() override;
    void mouseLeave() override;

    glm::mat4 modelMat;
};

template<typename UIElementType, typename... Args>
inline UIElementType* UIRoot::addUIElement(Args&&... args) {
    auto child = new UIElementType(std::forward<Args>(args)..., this);
    child->setup();
    children.push_back(child);
    return child;
}