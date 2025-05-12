#************#
# App Module #
#************#

add_library( app-module )
target_compile_definitions( app-module PRIVATE
        VKFW_NO_STRUCT_CONSTRUCTORS     # To enable designated initializers for VKFW structs
        VKFW_NO_INCLUDE_VULKAN_HPP
)

target_sources( app-module
        PUBLIC
            FILE_SET CXX_MODULES
            TYPE CXX_MODULES
            FILES
                app.ixx
                init.ixx
        PRIVATE
            app.cxx
            init.cxx
)

# Internal Libraries
target_link_libraries( app-module PUBLIC
        engine-module
        util-module
)

# External Dependencies
target_link_libraries( app-module PRIVATE
        vkfw::vkfw
        # vkfw-module
        vulkan_hpp-module
)
