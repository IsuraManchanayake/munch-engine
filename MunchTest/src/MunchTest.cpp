#include "Config/Config.h"

#include "App/App.h"
#include "Window/MainWindow.h"
#include "Window/EditorWindow.h"

int main() {
    App app;

    MainWindow* mainWindow = app.init<MainWindow>(800, 1000, 100, 100, "Hello Munch!");
    // MainWindow* mainWindow = app.init<MainWindow>(1720, 600, 100, 100, "Hello Munch!");
    // EditorWindow* editorWindow = app.addWindow<EditorWindow>(800, 600, 920, 100, "Hello UI Window!");

    app.setup();

    while(!app.shouldClose()) {
        app.pollEvents();
        app.update();
    }
    return 0;
}