#pragma once

#include "UI/AbstractUI.h"
#include "UI/UIMesh.h"
#include "Graphics/Texture.h"

struct UIElement : AbstractUI<UIElement> {
    using ParentType = AbstractUI<UIElement>;

    UIElement(int posX, int posY, int width, int height, ParentType* parent, UILayer* layer);
    virtual ~UIElement();

    virtual void setup() = 0;
    void setupParams();
    void calcTranslation();
    void render(glm::mat4 rootModel, GLint modelLocation);
    template<typename UIElementType, typename... Args>
    UIElementType* addUIElement(Args&&... args);
    virtual void mouseEnter() override;
    virtual void mouseInside() override;
    virtual void mouseLeave() override;
    void handleMouseMove(MouseMoveEvent& event) final;
    void handleMousePress(MousePressEvent& event) final;
    virtual bool isInside();
    void move(GLfloat dposX, GLfloat dposY, GLfloat drelX, GLfloat drelY);

    glm::mat4 translateMat;
    int relX, relY;
    UIMesh mesh;
    Texture texture;
    AbstractUI<UIElement>* parent;
    bool mouseAlreadyInside;
};

template<typename UIElementType, typename... Args>
inline UIElementType* UIElement::addUIElement(Args&&... args) {
    auto child = new UIElementType(std::forward<Args>(args)..., this, layer);
    child->setup();
    children.push_back(child);
    return child;
}