module;

#include "vkfw/vkfw.hpp"

export module app;

// import vkfw;
import vulkan_hpp;

export namespace app {
    constexpr vk::Extent2D WINDOW_SIZE{ 1280, 720 };    // Constant for tutorial usage

    class App
    {
    public:
        /* Execution */

        void run();

    private:
        /* Data Members */

        vkfw::Window m_window;
        vk::Instance m_vk_instance;

        /* Initialization Methods */

        /**
         * Initializes a GLFW instance and creates a window
         * @param window_size the 2D extent of the window, in pixels
         */
        void initWindow(const vk::Extent2D& window_size);

        /**
         * Creates a Vulkan instance with the required instance extensions
         */
        void initVulkan();

        /* Execution Methods */

        void mainLoop();

        /* Cleanup Methods */

        /**
         * Handles deletion of app-level objects, such as windows and the Vulkan instance
         */
        void cleanup();
    };
}
