#*******************#
# Vulkan Demo Setup #
#*******************#

add_executable( vulkan_demo
    main.cpp
)

target_link_libraries( vulkan_demo PRIVATE
        app-module
)

add_subdirectory( app )
add_subdirectory( engine )
add_subdirectory( util )