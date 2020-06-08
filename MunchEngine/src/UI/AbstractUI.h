#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

template<typename ChildrenType>
struct AbstractUI {
    AbstractUI(int posX, int posY, int width, int height);
    virtual ~AbstractUI();

    virtual void setup() = 0;
    virtual void mouseEnter() = 0;
    virtual void mouseInside() = 0;
    virtual void mouseLeave() = 0;
    
    int posX, posY;
    int width, height;
    glm::mat4 translateMat;
    unsigned id;
    static unsigned elemCount;
    std::vector<ChildrenType*> children;
};

template<typename ChildrenType>
inline unsigned AbstractUI<ChildrenType>::elemCount = 0;

template<typename ChildrenType>
inline AbstractUI<ChildrenType>::AbstractUI(int posX, int posY, int width, int height)
    : posX(posX), posY(posY), width(width), height(height), id(++elemCount), children() {
}

template<typename ChildrenType>
inline AbstractUI<ChildrenType>::~AbstractUI() {
    for(const auto& child : children) {
        delete child;
    }
}