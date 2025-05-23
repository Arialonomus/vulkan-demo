module;

#include <string_view>

export module pipeline;

import vulkan_hpp;

namespace eng::pipe {
    /* Pipeline Creation Methods */

    export [[nodiscard]] vk::PipelineLayout createPipelineLayout(const vk::Device& device);

    export [[nodiscard]] vk::Pipeline createGraphicsPipeline(const vk::Device& device,
                                                             const vk::PipelineLayout& layout,
                                                             const vk::PipelineRenderingCreateInfo& dynamic_rendering_info,
                                                             vk::PipelineCreateFlags flags = {});

    /* Creation Helper Methods */

    [[nodiscard]] vk::ShaderModule createShaderModule(const vk::Device& device,
                                                      std::string_view file_path,
                                                      vk::ShaderModuleCreateFlags flags = {});

    [[nodiscard]] vk::PipelineVertexInputStateCreateInfo configureVertexInputState();

    [[nodiscard]] vk::PipelineInputAssemblyStateCreateInfo configureInputAssemblyState();

    [[nodiscard]] vk::PipelineTessellationStateCreateInfo configureTessellationState();

    [[nodiscard]] vk::PipelineViewportStateCreateInfo configureViewportState();

    [[nodiscard]] vk::PipelineRasterizationStateCreateInfo configureRasterizationState();

    [[nodiscard]] vk::PipelineMultisampleStateCreateInfo configureMultisampleState();

    [[nodiscard]] vk::PipelineDepthStencilStateCreateInfo configureDepthStencilState();

    [[nodiscard]] vk::PipelineColorBlendStateCreateInfo configureColorBlendState();

    [[nodiscard]] vk::PipelineDynamicStateCreateInfo setAsDynamicState(std::span<const vk::DynamicState> state_values);
}
