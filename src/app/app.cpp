module;

#include "vkfw/vkfw.hpp"

module app;

// import vkfw;

namespace app {
    void App::run()
    {
        initWindow(WINDOW_SIZE);
        cleanup();
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

    void App::cleanup()
    {
        // Cleanup GLFW window
        m_window.destroy();
        vkfw::terminate();
    }
}