module;

#include "vkfw/vkfw.hpp"

module app;

// Internal Dependencies
import init;

namespace app {
    void App::run()
    {
        // Run app
        mainLoop();
    }

    void App::mainLoop()
    {
        while (!m_window.shouldClose()) {
            vkfw::pollEvents();
            m_engine.drawFrame();
        }
    }
}
