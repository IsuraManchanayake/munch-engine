#include <iostream>

#include "App/App.h"
#include "Window/MainWindow.h"

int main() {
    App app;

    MainWindow* mainWindow = app.init<MainWindow>(800, 600, 100, 100);
    // EditorWindow* editorWindow = app.addWindow<EditorWindow>(800, 600, 920, 100);

    app.setup();

    while(!app.shouldClose()) {
        app.pollEvents();

        app.update();
    }
    return 0;
}