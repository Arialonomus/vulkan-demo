module;

#include <vector>

export module init;

import vulkan_hpp;

namespace app::init {
    /* Vulkan Initialization Functions */

    export vk::Instance createVulkanInstance();

    /* Helper Functions */

    /**
     * Returns a list of required instance extensions not supported by the target Vulkan implementation, if any
     * @param targeted_extensions the list of targeted extensions required for the application
     * @return a std::vector containing the names (if any) of unsupported extensions, returns an empty vector if
     * all required extensions are supported
     */
    std::vector<const char*> getUnsupportedInstanceExtensions(const std::vector<const char*>& targeted_extensions);
}


