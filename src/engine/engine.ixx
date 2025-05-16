module;

#include "vkfw/vkfw.hpp"

export module engine;

// External Dependencies
// import vkfw;
import vulkan_hpp;

// Internal Dependencies
import init;
import gpu;
import swapchain;
import vulkan_utils;

namespace eng {
    export class Engine {
    public:
        /* Constructors */

        explicit Engine(const vkfw::Window& window);

    private:
        /* Data Members */

        GPU                     m_gpu;
        vk::SharedInstance      m_vk_instance;  // Stored for convenience, as Instance is owned by the Surface
        vk::SharedSwapchainKHR  m_swapchain;    // Swapchain owns Device and Surface (stored internally)
        vk::SharedDevice        m_device;

        std::vector<vk::SharedImage>        m_images;
        std::vector<vk::SharedImageView>    m_image_views;
    };
}
