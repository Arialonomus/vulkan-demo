module;

#include <array>
#include <string_view>

module pipeline;

// Internal Dependencies
import file_utils;

namespace eng::pipe {
    vk::PipelineLayout createPipelineLayout(const vk::Device& device)
    {
        return device.createPipelineLayout(vk::PipelineLayoutCreateInfo());
    }

    vk::Pipeline createGraphicsPipeline(const vk::Device& device,
                                        const vk::PipelineLayout& layout,
                                        const vk::PipelineRenderingCreateInfo& dynamic_rendering_info,
                                        vk::PipelineCreateFlags flags)
    {
        // Configure active shader stages
        const vk::ShaderModule vertex_shader_module{ createShaderModule(device, "shaders/vert.spv") };
        const auto vertex_shader = vk::PipelineShaderStageCreateInfo()
            .setStage( vk::ShaderStageFlagBits::eVertex )
            .setModule( vertex_shader_module )
            .setPName( "main" );

        const vk::ShaderModule fragment_shader_module{ createShaderModule(device, "shaders/frag.spv") };
        const auto fragment_shader = vk::PipelineShaderStageCreateInfo()
            .setStage( vk::ShaderStageFlagBits::eFragment )
            .setModule( fragment_shader_module )
            .setPName( "main" );

        const std::array shader_stages{ vertex_shader, fragment_shader };

        // Configure fixed-function stages
        const auto vertex_input_state{ configureVertexInputState() };
        const auto input_assembly_state{ configureInputAssemblyState() };
        const auto tessellation_state{ configureTessellationState() };
        const auto viewport_state{ configureViewportState() };
        const auto rasterization_state{ configureRasterizationState() };
        const auto multisample_state{ configureMultisampleState() };
        const auto depth_stencil_state{ configureDepthStencilState() };
        const auto color_blend_state{ configureColorBlendState() };

        // Designate dynamic pipeline state
        constexpr std::array dynamic_state_values{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };
        const auto dynamic_state{ setAsDynamicState(dynamic_state_values) };

        // Create the pipeline
        const auto create_info = vk::GraphicsPipelineCreateInfo()
            .setFlags( flags )
            .setStages( shader_stages )
            .setPVertexInputState( &vertex_input_state )
            .setPInputAssemblyState( &input_assembly_state )
            .setPTessellationState( &tessellation_state )
            .setPViewportState( &viewport_state )
            .setPRasterizationState( &rasterization_state )
            .setPMultisampleState( &multisample_state )
            .setPDepthStencilState( &depth_stencil_state )
            .setPColorBlendState( &color_blend_state )
            .setPDynamicState( &dynamic_state )
            .setLayout( layout )
            .setPNext( &dynamic_rendering_info );  // Dynamic rendering info must be attached in the pNext chain
        const auto pipeline = device.createGraphicsPipeline({}, create_info);
        if (pipeline.result != vk::Result::eSuccess)
            throw std::runtime_error("failed to create graphics pipeline");

        device.destroyShaderModule(vertex_shader_module);
        device.destroyShaderModule(fragment_shader_module);
        return pipeline.value;
    }

    vk::ShaderModule createShaderModule(const vk::Device& device,
                                        const std::string_view file_path,
                                        const vk::ShaderModuleCreateFlags flags)
    {
        const util::BytecodeBuffer shader_code{ util::readSPVFile(file_path) };
        return device.createShaderModule({ flags, shader_code });
    }

    vk::PipelineVertexInputStateCreateInfo configureVertexInputState()
    {
        return vk::PipelineVertexInputStateCreateInfo();
    }

    vk::PipelineInputAssemblyStateCreateInfo configureInputAssemblyState()
    {
        return vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology( vk::PrimitiveTopology::eTriangleList )
            .setPrimitiveRestartEnable( false );
    }

    vk::PipelineTessellationStateCreateInfo configureTessellationState()
    {
        return vk::PipelineTessellationStateCreateInfo();
    }

    vk::PipelineViewportStateCreateInfo configureViewportState()
    {
        // We set the viewport state dynamically, so we just provide the viewport and scissor count
        return vk::PipelineViewportStateCreateInfo()
            .setViewportCount( 1 )
            .setScissorCount( 1 );
    }

    vk::PipelineRasterizationStateCreateInfo configureRasterizationState()
    {
        return vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable( false )
            .setRasterizerDiscardEnable( false )
            .setPolygonMode( vk::PolygonMode::eFill )
            .setCullMode( vk::CullModeFlagBits::eBack )
            .setFrontFace( vk::FrontFace::eCounterClockwise )
            .setDepthBiasEnable( false );
    }

    vk::PipelineMultisampleStateCreateInfo configureMultisampleState()
    {
        return vk::PipelineMultisampleStateCreateInfo();
    }

    vk::PipelineDepthStencilStateCreateInfo configureDepthStencilState()
    {
        return vk::PipelineDepthStencilStateCreateInfo();
    }

    vk::PipelineColorBlendStateCreateInfo configureColorBlendState()
    {
        // Since we are using dynamic rendering, there will only be one color attachment state
        constexpr auto alpha_blending = vk::PipelineColorBlendAttachmentState()
            .setBlendEnable( true )
            .setSrcColorBlendFactor( vk::BlendFactor::eSrcAlpha )
            .setDstColorBlendFactor( vk::BlendFactor::eOneMinusSrcAlpha )
            .setColorBlendOp( vk::BlendOp::eAdd )
            .setSrcAlphaBlendFactor( vk::BlendFactor::eOne )
            .setDstAlphaBlendFactor( vk::BlendFactor::eZero )
            .setAlphaBlendOp( vk::BlendOp::eAdd )
            .setColorWriteMask( vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA );

        return vk::PipelineColorBlendStateCreateInfo()
            .setLogicOpEnable( false )
            .setAttachments( alpha_blending );
    }

    vk::PipelineDynamicStateCreateInfo setAsDynamicState(std::span<const vk::DynamicState> state_values)
    {
        return vk::PipelineDynamicStateCreateInfo()
            .setDynamicStates( state_values );
    }
}
