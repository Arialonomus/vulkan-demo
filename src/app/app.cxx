module;

#include "vkfw/vkfw.hpp"

module app;

// import vkfw;
import vulkan_hpp;

import init;

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
        m_vk_instance = init::createVulkanInstance();
        m_surface = vkfw::createWindowSurface(m_vk_instance, m_window);
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
        m_vk_instance.destroySurfaceKHR(m_surface);
        m_vk_instance.destroy();

        // Clean up GLFW resources
        m_window.destroy();
        vkfw::terminate();
    }
}
