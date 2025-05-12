module;

#include "vkfw/vkfw.hpp"

module app;

// Internal Dependencies
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
        // Instantiate the Vulkan instance
        m_vk_instance = init::createVulkanInstance();

        // Initialize execution model
        m_surface = vkfw::createWindowSurface(m_vk_instance, m_window);

        // Prepare the required device extension list
        const std::vector<const char*> required_extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        // Create the logical device
        m_gpu = init::selectSuitableGPU(m_vk_instance, required_extensions, m_surface);
        m_device = m_gpu.createLogicalDevice(required_extensions);

        // Get queue handles
        m_graphics_queue = m_device.getQueue(m_gpu.getGraphicsFamilyIndex(), 0);
        m_present_queue = m_device.getQueue(m_gpu.getPresentFamilyIndex(), 0);
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
        m_device.destroy();
        m_vk_instance.destroySurfaceKHR(m_surface);
        m_vk_instance.destroy();

        // Clean up GLFW resources
        m_window.destroy();
        vkfw::terminate();
    }
}
