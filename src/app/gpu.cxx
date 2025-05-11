module;

#import <vector>

module gpu;

import vulkan_utils;

namespace app {
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

    bool GPU::supportsRequiredExtensions(const std::vector<const char*>& required_extensions) const
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
}
