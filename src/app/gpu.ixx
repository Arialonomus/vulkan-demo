module;

#include <cstdint>
#include <optional>
#include <vector>

export module gpu;

import vulkan_hpp;

namespace app {
    /**
     * Represents the indices of Vulkan queue families that are needed for executing specific operations.
     * The indices are stored as optional values to allow verification of their assignment.
     */
    struct QueueFamilyIndices {
        std::optional<std::uint32_t> graphics{ std::nullopt };
        std::optional<std::uint32_t> present{ std::nullopt };

        [[nodiscard]] bool isFullyPopulated() const
        { return
            graphics.has_value()
            && present.has_value(); }
    };

    export class GPU
    {
    public:
        /* Constructors */

        // Default Constructor
        GPU() = default;

        // Standard Constructor
        explicit GPU(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
            : m_device{ device },
              m_properties{ device.getProperties() },
              m_features{ device.getFeatures() }
        {
            findQueueFamilies(surface);
        }

        /* Accessors */

        [[nodiscard]] std::uint32_t getGraphicsFamilyIndex() const
        { return m_queue_family_indices.graphics.value(); }

        [[nodiscard]] std::uint32_t getPresentFamilyIndex() const
        { return m_queue_family_indices.present.value(); }

        /* Device Functionality Queries */

        [[nodiscard]] bool isDiscreteGPU() const;

        [[nodiscard]] bool supportsGraphicsQueues() const;

        [[nodiscard]] bool supportsPresentationQueues() const;

        [[nodiscard]] bool supportsRequiredExtensions(const std::vector<const char*>& required_extensions) const;

        [[nodiscard]] bool meetsSwapChainRequirements(const vk::SurfaceKHR& surface) const;

    private:
        /* Data Members */

        vk::PhysicalDevice m_device;
        vk::PhysicalDeviceProperties m_properties;
        vk::PhysicalDeviceFeatures m_features;
        QueueFamilyIndices m_queue_family_indices;

        /* Helper Methods */

        /**
         * Stores the indices of the queue families for which we require support
         * @param surface the target surface for swap chain presentation, used to query Present Queue support
         */
        void findQueueFamilies(const vk::SurfaceKHR& surface);
    };
}
