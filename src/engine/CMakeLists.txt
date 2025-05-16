#***************#
# Engine Module #
#***************#

add_library( engine-module )

target_sources( engine-module
        PUBLIC
            FILE_SET CXX_MODULES
            TYPE CXX_MODULES
            FILES
                engine.ixx
                init.ixx
                gpu.ixx
                swapchain.ixx
        PRIVATE
            engine.cxx
            init.cxx
            gpu.cxx
            swapchain.cxx
)

# Internal Libraries
target_link_libraries( engine-module PRIVATE
        util-module
)

# External Dependencies
target_link_libraries( engine-module PRIVATE
        vkfw::vkfw
        # vkfw-module
        vulkan_hpp-module
)
