#***************#
# Project Setup #
#***************#

# CMake configuration
cmake_minimum_required( VERSION 3.31 )
set(VCPKG_OVERLAY_PORTS "${CMAKE_SOURCE_DIR}/vcpkg-ports")
set(VCPKG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cmake-build-debug-clang/vcpkg_installed/x64-linux/include")

# C++ Standard and Compiler Info
if ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    add_compile_options( -stdlib=libc++ )
    add_link_options( -stdlib=libc++ -lc++abi )
endif()
set( CMAKE_CXX_SCAN_FOR_MODULES ON )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Project
project( vulkan_demo LANGUAGES CXX )

#**************#
# Dependencies #
#**************#

# Locate dependencies
find_package( Vulkan REQUIRED ) # Vulkan SDK should be installed on disk, not managed by vcpkg
find_package( vkfw CONFIG REQUIRED )
find_package( glm CONFIG REQUIRED )

#**************************#
# Module Wrapper Libraries #
#**************************#

# Vulkan.hpp Module Wrapper Library
if(${Vulkan_VERSION} VERSION_LESS "1.3.256")
    message(FATAL_ERROR "Minimum required Vulkan version for C++ modules is 1.3.256. Found ${Vulkan_VERSION}")
endif()
add_library( vulkan_hpp-module )
target_sources( vulkan_hpp-module PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${Vulkan_INCLUDE_DIR}
        FILES ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
)
target_compile_features( vulkan_hpp-module PUBLIC cxx_std_20 )
target_link_libraries( vulkan_hpp-module PUBLIC Vulkan::Vulkan )

# VKFW Module Wrapper Library
set( VKFW_INCLUDE_DIR "${VCPKG_INCLUDE_DIR}/vkfw" )
add_library( vkfw-module )
target_sources( vkfw-module PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${VKFW_INCLUDE_DIR}
        FILES ${VKFW_INCLUDE_DIR}/vkfw.cppm
)
target_compile_features( vkfw-module PUBLIC cxx_std_20 )
target_link_libraries( vkfw-module PRIVATE
        vkfw::vkfw
        vulkan_hpp-module
)
target_compile_definitions( vkfw-module PRIVATE VKFW_ENABLE_VULKAN_HPP_MODULE )

# GLM Module Wrapper Library
set( GLM_INCLUDE_DIR "${VCPKG_INCLUDE_DIR}/glm" )
add_library( glm-module )
target_sources( glm-module PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${GLM_INCLUDE_DIR}
        FILES ${GLM_INCLUDE_DIR}/glm.cppm
)
target_compile_features( glm-module PUBLIC cxx_std_23 )
target_include_directories( glm-module PUBLIC ${GLM_INCLUDE_DIR} )

#****************#
# Subdirectories #
#****************#

add_subdirectory( src )