module;

#include "vkfw/vkfw.hpp"

export module app;

// External Dependencies
//import vkfw;

// Internal Dependencies
import engine;

namespace app {
    export class App
    {
    public:
        /* Constructor */

        App()
            : m_glfw_context{ },
              m_window{
                  1280,
                  720,
                  "Vulkan Demo",
                  {
                      .resizable = false,
                      .clientAPI = vkfw::ClientAPI::eNone
                  }
              },
              m_engine{ m_window }
        {}

        /* Program Execution Methods */

        void run();

    private:
        /* Data Members */

        vkfw::raii::Instance m_glfw_context;
        vkfw::raii::Window m_window;
        eng::Engine m_engine;

        /* Program Loop Methods */

        void mainLoop();
    };
}
