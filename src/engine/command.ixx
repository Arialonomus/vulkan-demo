module;

#include <cstdint>

export module command;

// External Dependencies
import vulkan_hpp;

namespace eng::cmd {
    /* Command Buffer Functions */

    export [[nodiscard]] vk::CommandBuffer
    allocateCommandBuffer(const vk::Device& device,
                          const vk::CommandPool& command_pool,
                          vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

    export void
    recordDrawCommand(const vk::CommandBuffer& command_buffer,
                      const vk::Pipeline& graphics_pipeline,
                      const vk::ImageView& target_image_view,
                      const vk::Image& target_image,
                      const vk::Extent2D& image_extent);
}
