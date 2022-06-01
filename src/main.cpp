#include "App.h"
#define WINDOW_TITLE "Mazer GUI"
#define WINDOW_DIMENSIONS 1200, 720

int main(int, char**) {
    App myApp(WINDOW_TITLE, WINDOW_DIMENSIONS);
    myApp.run();
    return 0;
}