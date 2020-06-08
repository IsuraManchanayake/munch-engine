#include "UI/UIElement.h"
#include "Graphics/Shader.h"
#include "Core/Logger.h"

UIElement::UIElement(int posX, int posY, int width, int height, ParentType* parent) 
    : AbstractUI<UIElement>(posX, posY, width, height), translateMat(1.f),
      relX(posX - parent->posX), relY(posY - parent->posY), mesh(), texture(), parent(parent) {
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
    Logger::warn(id, " mouse enter");
}

void UIElement::mouseInside() {
    Logger::warn(id, " mouse inside");
}

void UIElement::mouseLeave() {
    Logger::warn(id, " mouse leave");   
}
