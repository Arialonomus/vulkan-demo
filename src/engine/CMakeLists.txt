#***************#
# Engine Module #
#***************#

add_library( engine-module )

target_sources( engine-module
        PUBLIC
            FILE_SET CXX_MODULES
            TYPE CXX_MODULES
            FILES
                gpu.ixx
        PRIVATE
            gpu.cxx
)

target_link_libraries( engine-module PRIVATE
        util-module
)