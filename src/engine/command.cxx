module;

#include <stdexcept>

module command;

namespace eng::cmd {
    vk::CommandBuffer allocateCommandBuffer(const vk::Device& device,
                                            const vk::CommandPool& command_pool,
                                            vk::CommandBufferLevel level)
    {
        return device.allocateCommandBuffers({
            command_pool,
            level,
        })[0];
    }

    void recordDrawCommand(const vk::CommandBuffer& command_buffer,
                           const vk::Pipeline& graphics_pipeline,
                           const vk::ImageView& frame_target,
                           const vk::Extent2D& image_extent)
    {
        if (command_buffer.begin({}) != vk::Result::eSuccess)
            throw std::runtime_error("failed to begin command buffer recording");

        // Prepare dynamic rendering info
        vk::RenderingAttachmentInfo color_attachment{
            frame_target,
            vk::ImageLayout::eColorAttachmentOptimal,
        };
        color_attachment.setClearValue({{0.0f, 0.0f, 0.0f, 1.0f}});
        const vk::RenderingInfo rendering_info{
            vk::RenderingFlags{ },
            vk::Rect2D{ { 0, 0 }, image_extent },
            1,
            {},
            color_attachment
        };

        // Bind rendering state and pipeline
        command_buffer.beginRendering(rendering_info);
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline);

        // Dynamically set the viewport
        const auto [ width, height ] = image_extent;
        const vk::Viewport viewport{
            0.0f, 0.0f,
            static_cast<float>(width), static_cast<float>(height),
            0.0f, 1.0f
        };
        command_buffer.setViewport(0, 1, &viewport);

        // Dynamically set the scissor rectangle
        const vk::Rect2D scissor{
            { 0, 0 },
            image_extent
        };
        command_buffer.setScissor(0, 1, &scissor);

        // Draw call
        command_buffer.draw(3, 1, 0, 0);

        // Cleanup
        command_buffer.endRendering();
        command_buffer.end();   // Will throw error on failure
    }
}
