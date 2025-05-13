module;

#include <vector>

export module init;

// External Dependencies
import vulkan_hpp;

// Internal Dependencies
import gpu;

namespace eng::init {
    /* Vulkan Initialization Functions */

    /**
     * Validates if the target Vulkan implementation supports the application's needs and, if so, creates
     * a Vulkan instance object
     * @return a newly created Vulkan instance object configured for the program
     */
    export [[nodiscard]] vk::Instance
    createVulkanInstance();

    /**
     * Selects the best available Physical Device in the system and returns a corresponding GPU object
     * @param candidate_devices the list of Vulkan-compatible physical devices on the system
     * @param required_extensions the extensions required for the candidate device
     * @param surface the target surface for swap chain rendering
     * @return a newly instantiated GPU object corresponding to the best-suited Physical Device on the system
     */
    export [[nodiscard]] GPU
    selectSuitableGPU(std::span<const vk::PhysicalDevice> candidate_devices,
                      std::span<const char* const> required_extensions,
                      const vk::SurfaceKHR& surface);

    /* Helper Functions */

    /**
     * Accumulates all required instance extension names and validates they are supported before returning the list
     * of extension names to be used in an InstanceCreateInfo struct
     * @return a std::vector containing a list of enabled extension names
     * @throws std::runtime_error if any required extensions are not supported by the target vulkan implementation
     */
    [[nodiscard]] std::vector<const char*>
    enumerateEnabledInstanceExtensions();
}


