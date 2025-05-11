module;

#include <vector>

export module init;

import vulkan_hpp;

namespace app::init {
    /* Vulkan Initialization Functions */

    export vk::Instance createVulkanInstance();

    /* Helper Functions */

    /**
     * Accumulates all required instance extension names and validates they are supported before returning the list
     * of extension names to be used in an InstanceCreateInfo struct
     * @return a std::vector containing a list of enabled extension names
     * @throws std::runtime_error if any required extensions are not supported by the target vulkan implementation
     */
    std::vector<const char*> enumerateEnabledInstanceExtensions();

    /**
     * Returns a list of required instance extensions not supported by the target Vulkan implementation, if any
     * @param targeted_extensions the list of targeted extensions required for the application
     * @return a std::vector containing the names (if any) of unsupported extensions, returns an empty vector if
     * all required extensions are supported
     */
    std::vector<const char*> getUnsupportedInstanceExtensions(const std::vector<const char*>& targeted_extensions);
}


