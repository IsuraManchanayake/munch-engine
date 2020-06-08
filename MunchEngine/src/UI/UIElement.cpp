#include "UI/UIElement.h"
#include "Graphics/Shader.h"
#include "Core/Logger.h"
#include "UI/UILayer.h"

UIElement::UIElement(int posX, int posY, int width, int height, ParentType* parent, UILayer* layer) 
    : AbstractUI<UIElement>(posX, posY, width, height, layer), translateMat(1.f),
      relX(posX - parent->posX), relY(posY - parent->posY), mesh(), texture(), parent(parent), mouseAlreadyInside(false) {
}

UIElement::~UIElement() {
}

void UIElement::setupParams() {
    calcTranslation();
}

void UIElement::calcTranslation() {
    translateMat = glm::translate(glm::mat4{1.f}, {relX, relY, 0.f});
}

void UIElement::render(glm::mat4 rootModel, GLint modelLocation) {
    glm::mat4 absoluteModel = rootModel * translateMat;
    for(auto& child : children) {
        child->render(absoluteModel, modelLocation);
    }
    Shader::setm4(modelLocation, absoluteModel * glm::scale(glm::mat4{1.f}, {width, height, 1.f}));
    texture.use(GL_TEXTURE0);
    mesh.render();
}

void UIElement::mouseEnter() {
}

void UIElement::mouseInside() {
}

void UIElement::mouseLeave() {
}

void UIElement::handleMouseMove(MouseMoveEvent& event) {
    for(auto& child : children) {
        child->handleMouseMove(event);
    }
    bool inside = false;
    if(isInside()) {
        if(mouseAlreadyInside) {
            mouseInside();
            inside = true;
        } else {
            mouseEnter();
            mouseAlreadyInside = true;
        }
    } else {
        if(mouseAlreadyInside) {
            mouseLeave();
            mouseAlreadyInside = false;
        }
    }
    if(id == layer->currentDraggingUIElementId && !inside) {
        layer->currentDraggingUIElementId = 0;
    }
}

void UIElement::handleMousePress(MousePressEvent& event) {
    for(auto& child : children) {
        child->handleMousePress(event);
    }
    mousePress();
}

bool UIElement::isInside() {
    return (posX <= (layer->mouse.x - layer->mouse.dx)) && ((layer->mouse.x - layer->mouse.dx) < posX + width)
        && (posY <= (layer->mouse.y - layer->mouse.dy)) && ((layer->mouse.y - layer->mouse.dy) < posY + height);
}

void UIElement::move(GLfloat dposX, GLfloat dposY, GLfloat drelX, GLfloat drelY) {
    posX += dposX;
    posY += dposY;
    relX += drelX;
    relY += drelY;
    calcTranslation();
    for(auto& child : children) {
        child->move(dposX, dposY, 0.f, 0.f);
    }
}
