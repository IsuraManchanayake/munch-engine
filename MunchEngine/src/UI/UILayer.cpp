#include "UI/UILayer.h"
#include "UI/Types/PaneUI.h"

UILayer::UILayer(std::string name, int width, int height) 
    : Layer(name, width, height), shader(), root(0, 0, width, height) {
}

void UILayer::setup() {
    shader.create("Shaders/editor.vert.glsl", "Shaders/editor.frag.glsl");
    shader.use();
    shader.seti1("theTexture", 0);
    PaneUI* pane = root.addUIElement<PaneUI>(100, 100, 200, 200);
    root.setup();
}

void UILayer::update() {
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    root.render(shader.getUniformLocation("model"));
}