#include "App.h"
#include "settings.h"

int main(int, char**) {
    App myApp(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    myApp.run();
    return 0;
}