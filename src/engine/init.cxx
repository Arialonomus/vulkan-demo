module;

#include <vector>
#include <format>

#include "vkfw/vkfw.hpp"

module init;

// Internal Dependencies
import container_utils;
import vulkan_utils;

namespace eng::init {
    vk::Instance createVulkanInstance()
    {
        constexpr auto app_info = vk::ApplicationInfo()
            .setPApplicationName( "Hello Triangle" )
            .setApplicationVersion( vk::makeApiVersion(0, 0, 1, 0) )
            .setApiVersion( vk::ApiVersion14 );

        const auto enabled_extensions = enumerateEnabledInstanceExtensions();

        const auto instance_info = vk::InstanceCreateInfo()
            .setPApplicationInfo( &app_info )
            .setPEnabledExtensionNames( enabled_extensions );

        return vk::createInstance( instance_info );
    }

    GPU selectSuitableGPU(const std::span<const vk::PhysicalDevice> candidate_devices,
                          const std::span<const char* const> required_extensions,
                          const vk::SurfaceKHR& surface)
    {
        if (candidate_devices.empty())
            throw std::runtime_error("unable to locate a Vulkan-compatible GPU");

        // Filter any non-viable GPUs
        std::vector<GPU> viable_gpus{ };
        for (const auto& device : candidate_devices) {
            GPU gpu{ device, surface };
            const bool meets_minimum_requirements{
                gpu.supportsGraphicsQueues()
                && gpu.supportsPresentationQueues()
                && gpu.supportsRequiredExtensions(required_extensions)
                && gpu.meetsSwapChainRequirements(surface)
            };
            if (meets_minimum_requirements)
                viable_gpus.push_back(gpu);
        }
        if (viable_gpus.empty())
            throw std::runtime_error("unable to locate a suitable GPU");

        // Select the best GPU from the remaining candidates
        if (viable_gpus.size() == 1)
            return viable_gpus.front();

        return selectBestGPU(viable_gpus);
    }

    std::vector<const char*> enumerateEnabledInstanceExtensions()
    {
        // Query required instance extensions from external libraries
        const auto glfw_extensions = vkfw::getRequiredInstanceExtensions();

        // Accumulate required extension names into a single list
        constexpr std::array specified_extensions{ vk::EXTDebugUtilsExtensionName };
        const auto enabled_extensions{
            util::flattenToVector<const char*>(specified_extensions, glfw_extensions)
        };

        // Ensure all required extensions are supported
        const auto unsupported_extensions{
            util::getUnsupportedExtensions(enabled_extensions, vk::enumerateInstanceExtensionProperties()) };
        if (!unsupported_extensions.empty()) {
            throw std::runtime_error(std::format(
                "the following required instance extensions are not "
                "supported by the target Vulkan implementation:\n\t{}", unsupported_extensions
            ));
        }

        return enabled_extensions;
    }

    GPU selectBestGPU(const std::span<const GPU> candidate_gpus)
    {
        std::vector<std::pair<int, GPU>> gpu_scores(candidate_gpus.size());
        for (const auto& gpu : candidate_gpus) {
            int gpu_score{ 0 };

            // Discrete GPUs (i.e., video cards) perform better than integrated GPUs
            if (gpu.isDiscreteGPU())
                gpu_score += 500;

            // Performance is better when the Presentation and Graphics queue families are the same
            if (gpu.getPresentFamilyIndex() == gpu.getGraphicsFamilyIndex())
                gpu_score += 100;

            // CPUs should only be used as a fallback when no other device is present,
            // even if it scores the same as other devices
            if (gpu.isCPU())
                gpu_score = -1;

            gpu_scores.emplace_back(gpu_score, gpu);
        }

        // Sort the GPUs by their scores
        std::ranges::sort(gpu_scores, [](const auto& lhs, const auto& rhs) {
            return lhs.first > rhs.first;
        });
        return gpu_scores.front().second;
    }
}
