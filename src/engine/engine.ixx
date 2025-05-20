module;

#include "vkfw/vkfw.hpp"

export module engine;

// External Dependencies
// import vkfw;
import vulkan_hpp;

// Internal Dependencies
import init;
import gpu;
import vulkan_utils;

namespace eng {
    export class Engine {
    public:
        /* Constructors */

        explicit Engine(const vkfw::Window& window);

        /* Rendering Calls */

        void drawFrame();

    private:
        /* Data Members */

        GPU                     m_gpu;
        vk::SharedInstance      m_vk_instance;  // Stored for convenience, as Instance is owned by the Surface
        vk::SharedSwapchainKHR  m_swapchain;    // Swapchain owns Device and Surface (stored internally)
        vk::SharedDevice        m_device;

        std::vector<vk::SharedImage>        m_images;
        std::vector<vk::SharedImageView>    m_image_views;

        vk::SharedCommandPool   m_command_pool;
        vk::SharedCommandBuffer m_command_buffer;

        vk::Queue m_graphics_queue;
        vk::Queue m_present_queue;

        vk::SharedPipelineLayout m_pipeline_layout;
        vk::SharedPipeline       m_graphics_pipeline;

        vk::SharedSemaphore m_image_available;
        vk::SharedSemaphore m_render_finished;
        vk::SharedFence     m_in_flight;
    };
}
