module;

#include <vector>
#include <format>

#include "vkfw/vkfw.hpp"

module init;

// External Dependencies
// import vkfw;
import vulkan_hpp;

// Internal Dependencies
import container_utils;
import vulkan_utils;

namespace app::init {
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

    GPU selectSuitableGPU(const vk::Instance& instance, const vk::SurfaceKHR& surface)
    {
        const auto candidate_devices = instance.enumeratePhysicalDevices();
        if (candidate_devices.empty())
            throw std::runtime_error("unable to locate a Vulkan-compatible GPU");
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
