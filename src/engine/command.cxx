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
                           const vk::RenderingInfo& rendering_info)
    {
        if (command_buffer.begin({}) != vk::Result::eSuccess)
            throw std::runtime_error("failed to begin command buffer recording");

        command_buffer.beginRendering(rendering_info);
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline);

        // Dynamically set the viewport
        const auto window_extent = rendering_info.renderArea.extent;
        const auto [ width, height ] = window_extent;
        const vk::Viewport viewport{
            0.0f, 0.0f,
            static_cast<float>(width), static_cast<float>(height),
            0.0f, 1.0f
        };
        command_buffer.setViewport(0, 1, &viewport);

        // Dynamically set the scissor
        const vk::Rect2D scissor{
            { 0, 0 },
            window_extent
        };
        command_buffer.setScissor(0, 1, &scissor);

        command_buffer.draw(3, 1, 0, 0);

        command_buffer.endRendering();
        command_buffer.end();   // Will throw error on failure
    }
}
