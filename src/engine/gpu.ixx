module;

#include <cstdint>
#include <optional>
#include <vector>

export module gpu;

// External Dependencies
import vulkan_hpp;

namespace eng {
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

        /* Device Creation Methods */

        /**
         * Creates a logical device object corresponding to the passed-in GPU configured to the program needs
         * @param required_extensions the extensions required for the candidate device
         * @return a newly instantiated logical Device object configured for the program
         */
        [[nodiscard]] vk::Device createLogicalDevice(const std::vector<const char*>& required_extensions) const;

        /* Accessors */

        [[nodiscard]] const vk::PhysicalDevice& getDevice() const
        { return m_device; }

        [[nodiscard]] const vk::PhysicalDeviceProperties& getProperties() const
        { return m_properties; }

        [[nodiscard]] const vk::PhysicalDeviceFeatures& getFeatures() const
        { return m_features; }

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
