module;

#include "vkfw/vkfw.hpp"

module engine;

// Internal Dependencies
import swapchain;
import pipeline;

namespace eng {
    Engine::Engine(const vkfw::Window& window)
            : m_vk_instance{ init::createVulkanInstance() }
    {
        // Select the candidate GPU and create the logical device
        const auto candidate_devices{ m_vk_instance->enumeratePhysicalDevices() };
        const std::vector required_device_extensions{
            vk::KHRSwapchainExtensionName,
            vk::KHRDynamicRenderingExtensionName,
        };
        const vk::SharedSurfaceKHR surface{ vkfw::createWindowSurface(m_vk_instance, window), m_vk_instance };
        m_gpu = init::selectSuitableGPU(candidate_devices, required_device_extensions, surface);
        m_device = vk::SharedDevice{ m_gpu.createLogicalDevice(required_device_extensions) };

        // Create the swapchain
        const auto [color_format, extent, swapchain, images, image_views]{
            swap::createSwapchain(m_gpu,
                                  m_device,
                                  surface,
                                  util::toExtent2D(window.getFramebufferSize()),
                                  vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc ) };
        m_swapchain = vk::SharedSwapchainKHR{ swapchain, m_device, surface };

        // Convert the swapchain images and views to shared handles
        m_images.reserve(images.size());
        std::ranges::transform( images,
                                std::back_inserter(m_images),
                                [this](const vk::Image image ) {
                                    return vk::SharedImage{ image, m_device, vk::SwapchainOwns::yes };
                                } );

        m_image_views.reserve(image_views.size());
        std::ranges::transform( image_views,
                                std::back_inserter(m_image_views),
                                [this](const vk::ImageView image_view ) {
                                    return vk::SharedImageView{ image_view, m_device };
                                } );

        // Generate the queue handles
        m_graphics_queue = m_device->getQueue(m_gpu.getGraphicsFamilyIndex(), 0);
        m_present_queue = m_device->getQueue(m_gpu.getPresentFamilyIndex(), 0);

        // Create the graphics pipeline object
        const vk::PipelineRenderingCreateInfoKHR dynamic_rendering_info{
            {},
            color_format
        };
        m_pipeline_layout = vk::SharedPipelineLayout{ pipe::createPipelineLayout(m_device), m_device };
        m_graphics_pipeline = vk::SharedPipeline{
            pipe::createGraphicsPipeline(m_device,
                                         m_pipeline_layout,
                                         extent,
                                         dynamic_rendering_info),
            m_device
        };
    }
}
