#pragma once

#include "UI/AbstractUI.h"
#include "UI/UIElement.h"

struct UIRoot final : AbstractUI<UIElement> {
    UIRoot(int posX, int posY, int width, int height, UILayer* layer);
    ~UIRoot();

    void setup() override;
    void render(GLint modelLocation);
    template<typename UIElementType, typename... Args>
    UIElementType* addUIElement(Args&&... args);
    void calcModel();
    void mouseEnter() override;
    void mouseInside() override;
    void mouseLeave() override;
    void mousePress() override;
    void handleMouseMove(MouseMoveEvent& event) final;
    void handleMousePress(MousePressEvent& event) final;

    glm::mat4 modelMat;
};

template<typename UIElementType, typename... Args>
inline UIElementType* UIRoot::addUIElement(Args&&... args) {
    auto child = new UIElementType(std::forward<Args>(args)..., this, layer);
    child->setup();
    children.push_back(child);
    return child;
}