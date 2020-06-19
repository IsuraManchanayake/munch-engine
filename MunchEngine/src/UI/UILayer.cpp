#include "UI/UILayer.h"
#include "UI/Types/PaneUI.h"

UILayer::UILayer(std::string name, int width, int height) 
    : Layer(name, width, height), shader(), root(0, 0, width, height, this), currentDraggingUIElementId(0) {
}

void UILayer::setup() {
    shader.create()
        ->addShaderFromPath(ShaderType::Vertex, "Shaders/editor.vert.glsl")
        ->addShaderFromPath(ShaderType::Fragment, "Shaders/editor.frag.glsl")
        ->compile();
    shader.use();
    shader.seti1("theTexture", 0);
    PaneUI* pane1 = root.addUIElement<PaneUI>(100, 100, 200, 200);
    PaneUI* pane2 = pane1->addUIElement<PaneUI>(200, 200, 200, 300);
    root.setup();
}

void UILayer::update() {
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    root.render(shader.getUniformLocation("model"));
}

bool UILayer::onMouseMove(MouseMoveEvent& event) {
    mouse.x = event.x;
    mouse.y = event.y;
    mouse.dx = event.dx;
    mouse.dy = event.dy;
    root.handleMouseMove(event);
    return false;
}

bool UILayer::onMousePress(MousePressEvent& event) {
    mouse.button = event.button;
    mouse.action = event.action;
    root.handleMousePress(event);
    return false;
}

bool UILayer::onWindowResize(WindowResizeEvent& event) {
    width = event.width;
    height = event.height;
    root.width = width;
    root.height = height;
    root.calcModel();
    return false;
}