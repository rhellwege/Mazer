#include "App.h"
#include "settings.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void emMainLoop(void* data) {
    App* appInstance = static_cast<App*>(data);
    appInstance->mainLoop();
}
#endif

int main(int, char**) {
    App myApp(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    #ifndef __EMSCRIPTEN__
    myApp.run();
    #else
    emscripten_set_main_loop_arg(emMainLoop, (void*)&myApp, 0, true);
    #endif
    return 0;
}
