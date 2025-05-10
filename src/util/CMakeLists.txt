#************************#
# Utility Library Module #
#************************#

add_library( util-module )

target_sources( util-module
        PUBLIC
            FILE_SET CXX_MODULES
        FILES
            concepts.cppm
            list_utils.cppm
)