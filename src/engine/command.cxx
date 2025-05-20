module;

#include <stdexcept>

module command;

namespace eng::cmd {
    vk::CommandBuffer allocateCommandBuffer(const vk::Device& device,
                                            const vk::CommandPool& command_pool,
                                            const vk::CommandBufferLevel level)
    {
        return device.allocateCommandBuffers(vk::CommandBufferAllocateInfo()
            .setCommandPool( command_pool )
            .setLevel( level )
            .setCommandBufferCount( 1 )
        )[0];
    }

    void recordDrawCommand(const vk::CommandBuffer& command_buffer,
                           const vk::Pipeline& graphics_pipeline,
                           const vk::ImageView& target_image_view,
                           const vk::Image& target_image,
                           const vk::Extent2D& image_extent)
    {
        constexpr vk::CommandBufferBeginInfo begin_info{ };
        if (command_buffer.begin(&begin_info) != vk::Result::eSuccess)
            throw std::runtime_error("failed to begin command buffer recording");

        // Manually transition image layout for rendering
        const auto rendering_image_barrier = vk::ImageMemoryBarrier()
            .setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite )
            .setOldLayout( vk::ImageLayout::eUndefined )
            .setNewLayout( vk::ImageLayout::eColorAttachmentOptimal )
            .setImage( target_image )
            .setSubresourceRange( vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( 1 )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 ) );
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                       vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                       vk::DependencyFlags{ },
                                       {},
                                       {},
                                       rendering_image_barrier);

        // Prepare dynamic rendering info
        const auto color_attachment = vk::RenderingAttachmentInfo()
            .setImageView( target_image_view )
            .setImageLayout( vk::ImageLayout::eColorAttachmentOptimal )
            .setLoadOp( vk::AttachmentLoadOp::eClear )
            .setStoreOp( vk::AttachmentStoreOp::eStore )
            .setClearValue( {{0.0f, 0.0f, 0.0f, 1.0f}} );

        const auto rendering_info = vk::RenderingInfo()
            .setRenderArea( vk::Rect2D{{0, 0}, image_extent} )
            .setLayerCount( 1 )
            .setColorAttachments( color_attachment );

        // Bind rendering state and pipeline
        command_buffer.beginRendering(rendering_info);
        command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphics_pipeline);

        // Dynamically set the viewport
        const auto viewport = vk::Viewport()
            .setX( 0.0f )
            .setY( 0.0f )
            .setWidth( static_cast<float>(image_extent.width) )
            .setHeight( static_cast<float>(image_extent.height) )
            .setMinDepth( 0.0f )
            .setMaxDepth( 1.0f );
        command_buffer.setViewport(0, viewport);

        // Dynamically set the scissor rectangle
        const auto scissor = vk::Rect2D()
            .setOffset( {0, 0} )
            .setExtent( image_extent );
        command_buffer.setScissor(0, scissor);

        // Draw call
        command_buffer.draw(3, 1, 0, 0);

        // Cleanup
        command_buffer.endRendering();
        const auto presentation_image_barrier = vk::ImageMemoryBarrier()
            .setSrcAccessMask( vk::AccessFlagBits::eColorAttachmentWrite )
            .setOldLayout( vk::ImageLayout::eColorAttachmentOptimal )
            .setNewLayout( vk::ImageLayout::ePresentSrcKHR )
            .setImage( target_image )
            .setSubresourceRange( vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( 1 )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 ) );
        command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                       vk::PipelineStageFlagBits::eBottomOfPipe,
                                       vk::DependencyFlags{ },
                                       {},
                                       {},
                                       presentation_image_barrier);
        command_buffer.end();   // Will throw error on failure
    }
}
