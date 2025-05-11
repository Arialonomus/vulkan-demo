module;

#include <vector>
#include <format>

#include "vkfw/vkfw.hpp"

module app;

// import vkfw;
import vulkan_hpp;

import list_utils;

namespace app {
    void App::run()
    {
        // Initialize program state
        initWindow(WINDOW_SIZE);
        initVulkan();

        // Run app
        mainLoop();

        // Handle cleanup
        cleanup();
    }

    void App::initVulkan()
    {
        constexpr vk::ApplicationInfo app_info(
            "Hello Triangle",
            vk::makeApiVersion(0, 0, 1, 0),
            "No Engine",
            vk::makeApiVersion(0, 0, 0, 0),
            vk::ApiVersion14
        );

        // List desired layers
        constexpr std::vector<const char*> enabled_layers{ };

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

        // Create the Vulkan instance
        constexpr vk::InstanceCreateFlags flags{ };
        const vk::InstanceCreateInfo instance_info(
            flags,
            &app_info,
            enabled_layers,
            enabled_extensions
        );
        m_vk_instance = vk::createInstance(instance_info);
    }

    void App::initWindow(const vk::Extent2D& window_size)
    {
        vkfw::init();

        const vkfw::WindowHints hints{
            .resizable = false,
            .clientAPI = vkfw::ClientAPI::eNone
        };
        const auto [ width, height ] = window_size;
        m_window = vkfw::createWindow(width, height, "Vulkan Demo", hints);
    }

    void App::mainLoop()
    {
        while (!m_window.shouldClose()) {
            vkfw::pollEvents();
        }
    }

    void App::cleanup()
    {
        // Clean up Vulkan resources
        m_vk_instance.destroy();

        // Clean up GLFW resources
        m_window.destroy();
        vkfw::terminate();
    }

    std::vector<const char*> App::getUnsupportedInstanceExtensions(const std::vector<const char*>& targeted_extensions)
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
