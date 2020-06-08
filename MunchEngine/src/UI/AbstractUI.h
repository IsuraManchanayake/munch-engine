#pragma once

#include "Event/Types/MouseMoveEvent.h"
#include "Event/Types/MousePressEvent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct UILayer;

template<typename ChildrenType>
struct AbstractUI {
    AbstractUI(int posX, int posY, int width, int height, UILayer* layer);
    virtual ~AbstractUI();

    virtual void setup() = 0;
    virtual void mouseEnter() = 0;
    virtual void mouseInside() = 0;
    virtual void mouseLeave() = 0;
    virtual void mousePress() = 0;
    virtual void handleMouseMove(MouseMoveEvent& event) = 0;
    virtual void handleMousePress(MousePressEvent& event) = 0;

    void onMouseMove();
    
    int posX, posY;
    int width, height;
    glm::mat4 translateMat;
    unsigned id;
    static unsigned elemCount;
    UILayer* layer;
    std::vector<ChildrenType*> children;
};

template<typename ChildrenType>
inline unsigned AbstractUI<ChildrenType>::elemCount = 0;

template<typename ChildrenType>
inline AbstractUI<ChildrenType>::AbstractUI(int posX, int posY, int width, int height, UILayer* layer)
    : posX(posX), posY(posY), width(width), height(height), id(++elemCount), children(), layer(layer) {
}

template<typename ChildrenType>
inline AbstractUI<ChildrenType>::~AbstractUI() {
    for(const auto& child : children) {
        delete child;
    }
}