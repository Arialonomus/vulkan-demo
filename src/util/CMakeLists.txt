#************************#
# Utility Library Module #
#************************#

add_library( util-module )

target_sources( util-module
        PUBLIC
            FILE_SET CXX_MODULES
        FILES
            concepts.cxxm
            container_utils.cxxm
            vulkan_utils.cxxm
            file_utils.cxxm
)

target_link_libraries( util-module PRIVATE
        vulkan_hpp-module
)