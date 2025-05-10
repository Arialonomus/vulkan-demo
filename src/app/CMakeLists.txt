#************#
# App Module #
#************#

add_library( app-module )

target_sources( app-module
        PUBLIC
            FILE_SET CXX_MODULES
            FILES
                app.ixx
        PRIVATE
            app.cpp
)