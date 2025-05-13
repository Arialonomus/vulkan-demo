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
        constexpr vk::ApplicationInfo app_info(
            "Hello Triangle",
            vk::makeApiVersion(0, 0, 1, 0),
            "No Engine",
            vk::makeApiVersion(0, 0, 0, 0),
            vk::ApiVersion14
        );

        constexpr std::vector<const char*> enabled_layers{ };
        const std::vector<const char*> enabled_extensions = enumerateEnabledInstanceExtensions();
        constexpr vk::InstanceCreateFlags flags{ };

        const vk::InstanceCreateInfo instance_info(
            flags,
            &app_info,
            enabled_layers,
            enabled_extensions
        );
        return vk::createInstance(instance_info);
    }

    GPU selectSuitableGPU(const std::span<const vk::PhysicalDevice> candidate_devices,
                          const std::span<const char* const> required_extensions,
                          const vk::SurfaceKHR& surface)
    {
        if (candidate_devices.empty())
            throw std::runtime_error("unable to locate a Vulkan-compatible GPU");

        // Locate the best GPU
        std::optional<GPU> best_gpu{ std::nullopt };
        for (const auto& device : candidate_devices) {
            GPU gpu{ device, surface };
            const bool meets_minimum_requirements{
                gpu.supportsGraphicsQueues()
                && gpu.supportsPresentationQueues()
                && gpu.supportsRequiredExtensions(required_extensions)
                && gpu.meetsSwapChainRequirements(surface)
            };
            if (meets_minimum_requirements)
                best_gpu = gpu;
            if (gpu.isDiscreteGPU() || gpu.getGraphicsFamilyIndex() == gpu.getPresentFamilyIndex())
                best_gpu = gpu;
            if (gpu.isDiscreteGPU() && gpu.getGraphicsFamilyIndex() == gpu.getPresentFamilyIndex())
                best_gpu = gpu;
        }

        if (!best_gpu.has_value())
            throw std::runtime_error("unable to locate a suitable GPU");

        return best_gpu.value();
    }

    std::vector<const char*> enumerateEnabledInstanceExtensions()
    {
        // Query required instance extensions
        const auto glfw_extensions = vkfw::getRequiredInstanceExtensions();

        // Accumulate required extension names into a single list
        const auto enabled_extensions{
            util::flattenToVector<const char*>(glfw_extensions)
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
}
