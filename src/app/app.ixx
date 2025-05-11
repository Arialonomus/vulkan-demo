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

        /* Helper Methods */

        /**
         * Returns a list of required instance extensions not supported by the target Vulkan implementation, if any
         * @param targeted_extensions the list of targeted extensions required for the application
         * @return a std::vector containing the names (if any) of unsupported extensions, returns an empty vector if
         * all required extensions are supported
         */
        static std::vector<const char*> getUnsupportedInstanceExtensions(const std::vector<const char*>& targeted_extensions);
    };
}
