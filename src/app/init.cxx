module;

#include <vector>
#include <format>

#include "vkfw/vkfw.hpp"

module init;

// import vkfw;
import vulkan_hpp;

import list_utils;

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

    std::vector<const char*> enumerateEnabledInstanceExtensions()
    {
        // Query required instance extensions
        const auto glfw_extensions = vkfw::getRequiredInstanceExtensions();

        // Accumulate required extension names into a single list
        const auto enabled_extensions{
            util::flattenToVector<const char*>(glfw_extensions)
        };

        // Ensure all required extensions are supported
        const auto unsupported_extensions = getUnsupportedInstanceExtensions(enabled_extensions);
        if (!unsupported_extensions.empty()) {
            throw std::runtime_error(std::format(
                "the following required instance extensions are not "
                "supported by the target Vulkan implementation:\n\t{}", unsupported_extensions
            ));
        }

        return enabled_extensions;
    }

    std::vector<const char*> getUnsupportedInstanceExtensions(const std::vector<const char*>& targeted_extensions)
    {
        std::vector<const char*> unsupported_extensions{ targeted_extensions };
        const std::vector<vk::ExtensionProperties> supported_extensions{ vk::enumerateInstanceExtensionProperties() };

        // Remove supported extensions from the list until it is empty or all supported extensions have been checked
        auto current_extension{ supported_extensions.begin() };
        while (current_extension != supported_extensions.end() && !unsupported_extensions.empty()) {
            const char* extension_name{ current_extension->extensionName };
            std::erase_if(unsupported_extensions,
                          [extension_name](const char* targeted) {
                              return std::strcmp(extension_name, targeted) == 0;
                          });
            ++current_extension;
        }

        return unsupported_extensions;
    }
}
