module;

#include "vkfw/vkfw.hpp"

module app;

// import vkfw;
import vulkan_hpp;

import list_utils;

namespace app {
    void App::run()
    {
        // Initialize program state
        initWindow(WINDOW_SIZE);
        initVulkan();

        // Run app
        mainLoop();

        // Handle cleanup
        cleanup();
    }

    void App::initVulkan()
    {
        const vk::ApplicationInfo app_info(
            "Hello Triangle",
            vk::makeApiVersion(0, 0, 1, 0),
            "No Engine",
            vk::makeApiVersion(0, 0, 0, 0),
            vk::ApiVersion14
        );

        // List desired layers
        const std::vector<const char*> enabled_layers{ };

        // Query required instance extensions
        const auto glfw_extensions = vkfw::getRequiredInstanceExtensions();

        // Accumulate required extension names into a single list
        const auto enabled_extensions{
            util::flattenToVector<const char*>(glfw_extensions)
        };

        // Create the Vulkan instance
        const vk::InstanceCreateInfo instance_info(
            {},
            &app_info,
            enabled_layers,
            enabled_extensions
        );
        m_vk_instance = vk::createInstance(instance_info);
    }

    void App::initWindow(const vk::Extent2D& window_size)
    {
        vkfw::init();

        const vkfw::WindowHints hints{
            .resizable = false,
            .clientAPI = vkfw::ClientAPI::eNone
        };
        const auto [ width, height ] = window_size;
        m_window = vkfw::createWindow(width, height, "Vulkan Demo", hints);
    }

    void App::mainLoop()
    {
        while (!m_window.shouldClose()) {
            vkfw::pollEvents();
        }
    }

    void App::cleanup()
    {
        // Clean up Vulkan resources
        m_vk_instance.destroy();

        // Clean up GLFW resources
        m_window.destroy();
        vkfw::terminate();
    }
}