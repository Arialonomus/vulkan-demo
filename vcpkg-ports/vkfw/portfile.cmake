set(VCPKG_BUILD_TYPE release)

vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO Arialonomus/vkfw
        REF module-install
        SHA512 39adf71365b0ed7df3b1dbc9b65f0249dc469b780c1fb76bb28eba7a7307156ed701e649e721affc820d743523ef7103fa8dc9b8d938b5e3f7a59ed008baac52
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
            -DVKFW_INSTALL=ON
)

# Install and manually create the config file
set(VCPKG_INSTALL_HEADER_ONLY ON)
vcpkg_cmake_install()
file(INSTALL
        "${SOURCE_PATH}/cmake/vkfwConfig.cmake"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/vkfw"
)

# Export the targets
vcpkg_cmake_config_fixup(
        PACKAGE_NAME vkfw
        CONFIG_PATH lib/cmake/vkfw
)

# Fetch license
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

# Clean up unused lib/debug folders
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug" "${CURRENT_PACKAGES_DIR}/lib" "${CURRENT_PACKAGES_DIR}/debug/lib")
