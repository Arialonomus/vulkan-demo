module;

#import <vector>

module gpu;

// Internal Dependencies
import vulkan_utils;

namespace eng {
    vk::Device GPU::createLogicalDevice(const std::span<const char* const> required_extensions) const
    {
        std::vector<vk::DeviceQueueCreateInfo> queues;

        // Instantiate graphics queue
        constexpr std::array graphics_priorities{ 1.0f };
        queues.push_back(addDeviceQueue(this->getGraphicsFamilyIndex(), graphics_priorities));

        // Handle case where Graphics and Present queue are different families
        if (this->getPresentFamilyIndex() != this->getGraphicsFamilyIndex()) {
            constexpr std::array present_priorities{ 1.0f };
            queues.push_back(addDeviceQueue(this->getPresentFamilyIndex(), present_priorities));
        }

        // Instantiate logical device
        constexpr vk::PhysicalDeviceDynamicRenderingFeatures dynamic_rendering_enabled{ true };
        const auto device_info = vk::DeviceCreateInfo()
            .setQueueCreateInfos( queues )
            .setPEnabledExtensionNames( required_extensions )
            .setPEnabledFeatures( &this->getFeatures() )
            .setPNext( &dynamic_rendering_enabled );
        return m_device.createDevice(device_info);
    }

    bool GPU::isCPU() const
    {
        return m_properties.deviceType == vk::PhysicalDeviceType::eCpu;
    }

    bool GPU::isDiscreteGPU() const
    {
        return m_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
    }

    bool GPU::supportsGraphicsQueues() const
    {
        return m_queue_family_indices.graphics.has_value();
    }

    bool GPU::supportsPresentationQueues() const
    {
        return m_queue_family_indices.present.has_value();
    }

    bool GPU::supportsRequiredExtensions(const std::span<const char* const> required_extensions) const
    {
        const auto supported_extensions{ m_device.enumerateDeviceExtensionProperties() };
        return util::getUnsupportedExtensions(required_extensions, supported_extensions).empty();
    }

    bool GPU::meetsSwapChainRequirements(const vk::SurfaceKHR& surface) const
    {
        return !m_device.getSurfaceFormatsKHR(surface).empty()
            && !m_device.getSurfacePresentModesKHR(surface).empty();
    }

    void GPU::findQueueFamilies(const vk::SurfaceKHR& surface)
    {
        const auto queue_families{ m_device.getQueueFamilyProperties() };
        for (int i = 0; i < queue_families.size() && !m_queue_family_indices.isFullyPopulated(); ++i) {
            if (queue_families.at(i).queueFlags & vk::QueueFlagBits::eGraphics)
                m_queue_family_indices.graphics = i;
            if (m_device.getSurfaceSupportKHR(i, surface))
                m_queue_family_indices.present = i;
        }
    }

    vk::DeviceQueueCreateInfo GPU::addDeviceQueue(const uint32_t family_index,
                                                  const std::span<const float> priorities,
                                                  const vk::DeviceQueueCreateFlags flags) const
    {
        return vk::DeviceQueueCreateInfo()
            .setFlags(flags)
            .setQueueFamilyIndex(family_index)
            .setQueuePriorities(priorities);
    }
}
