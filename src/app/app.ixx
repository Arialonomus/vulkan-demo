module;

#include "vkfw/vkfw.hpp"

export module app;

// External Dependencies
// import vkfw;
import vulkan_hpp;

// Internal Dependencies
import gpu;

namespace app {
    export constexpr vk::Extent2D WINDOW_SIZE{ 1280, 720 };    // Constant for tutorial usage

    export class App
    {
    public:
        /* Program Execution Methods */

        void run();

    private:
        /* Data Members */

        vkfw::Window m_window;
        vk::Instance m_vk_instance;
        vk::SurfaceKHR m_surface;
        GPU m_gpu;
        vk::Device m_device;

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

        /* Program Loop Methods */

        void mainLoop();

        /* Cleanup Methods */

        /**
         * Handles deletion of app-level objects, such as windows and the Vulkan instance
         */
        void cleanup();
    };
}
