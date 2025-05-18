module;

#include <array>
#include <string_view>

module pipeline;

import file_utils;

namespace eng::pipe {
    vk::PipelineLayout createPipelineLayout(const vk::Device& device)
    {
        return device.createPipelineLayout(vk::PipelineLayoutCreateInfo{ });
    }

    vk::Pipeline createGraphicsPipeline(const vk::Device& device,
                                        const vk::PipelineLayout& layout,
                                        const vk::Extent2D& swapchain_extent,
                                        const vk::PipelineRenderingCreateInfoKHR& dynamic_rendering_info,
                                        vk::PipelineCreateFlags flags)
    {
        // Configure active shader stages
        const vk::ShaderModule vertex_shader_module{ createShaderModule(device, "shaders/vert.spv") };
        const vk::PipelineShaderStageCreateInfo vertex_shader(
            vk::PipelineShaderStageCreateFlags{ },
            vk::ShaderStageFlagBits::eVertex,
            vertex_shader_module,
            "main"
        );

        const vk::ShaderModule fragment_shader_module{ createShaderModule(device, "shaders/frag.spv") };
        const vk::PipelineShaderStageCreateInfo fragment_shader(
            vk::PipelineShaderStageCreateFlags{ },
            vk::ShaderStageFlagBits::eFragment,
            fragment_shader_module,
            "main"
        );

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
        constexpr std::array dynamic_state_values{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        const auto dynamic_state{ setAsDynamicState(dynamic_state_values) };

        // Create the pipeline
        vk::GraphicsPipelineCreateInfo create_info{
            flags,
            shader_stages,
            &vertex_input_state,
            &input_assembly_state,
            &tessellation_state,
            &viewport_state,
            &rasterization_state,
            &multisample_state,
            &depth_stencil_state,
            &color_blend_state,
            &dynamic_state,
            layout
        };
        create_info.setPNext(&dynamic_rendering_info);  // Dynamic rendering info must be attached in the pNext chain
        if (const auto result{ device.createGraphicsPipeline({}, create_info) }; result.result == vk::Result::eSuccess) {
            device.destroyShaderModule(vertex_shader_module);
            device.destroyShaderModule(fragment_shader_module);
            return result.value;
        }

        throw std::runtime_error("failed to create graphics pipeline");
    }

    vk::ShaderModule createShaderModule(const vk::Device& device,
                                        const std::string_view file_path,
                                        const vk::ShaderModuleCreateFlags flags)
    {
        const util::BytecodeBuffer shader_code{ util::readSPVFile(file_path) };
        return device.createShaderModule(vk::ShaderModuleCreateInfo(
            flags,
            shader_code
        ));
    }

    vk::PipelineVertexInputStateCreateInfo configureVertexInputState()
    {
        return { };
    }

    vk::PipelineInputAssemblyStateCreateInfo configureInputAssemblyState()
    {
        return {
            vk::PipelineInputAssemblyStateCreateFlags{ },
            vk::PrimitiveTopology::eTriangleList,
            false
        };
    }

    vk::PipelineTessellationStateCreateInfo configureTessellationState()
    {
        return { };
    }

    vk::PipelineViewportStateCreateInfo configureViewportState(const vk::Extent2D& swapchain_extent)
    {
        const vk::Viewport viewport{
            0, 0,
            static_cast<float>(swapchain_extent.width), static_cast<float>(swapchain_extent.height),
            0, 1 };
        const vk::Rect2D scissor{ vk::Offset2D{ 0, 0 }, swapchain_extent };

        return {
            vk::PipelineViewportStateCreateFlags{ },
            viewport,
            scissor
        };
    }

    vk::PipelineRasterizationStateCreateInfo configureRasterizationState()
    {
        return {
            vk::PipelineRasterizationStateCreateFlags{ },
            false,
            false,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eCounterClockwise,
            false
        };
    }

    vk::PipelineMultisampleStateCreateInfo configureMultisampleState()
    {
        return { };
    }

    vk::PipelineDepthStencilStateCreateInfo configureDepthStencilState()
    {
        return { };
    }

    vk::PipelineColorBlendStateCreateInfo configureColorBlendState()
    {
        // Since we are using dynamic rendering, there will only be one color attachment state
        constexpr vk::PipelineColorBlendAttachmentState alpha_blending{
            true,
            vk::BlendFactor::eSrcAlpha,
            vk::BlendFactor::eOneMinusSrcAlpha,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        return {
            vk::PipelineColorBlendStateCreateFlags{ },
            false,
            {},     // Empty, since we are not using a bitwise operation
            alpha_blending
        };
    }

    vk::PipelineDynamicStateCreateInfo setAsDynamicState(std::span<const vk::DynamicState> state_values)
    {
        return {
            vk::PipelineDynamicStateCreateFlags{ },
            state_values
        };
    }
}
