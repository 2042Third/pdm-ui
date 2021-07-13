cmake_minimum_required(VERSION 3.1...3.17)

if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    # If no deployment target has been set default to the minimum supported
    # OS version (this has to be set before the first project() call)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0 CACHE STRING "iOS Deployment Target")
    else()
        set(CMAKE_OSX_DEPLOYMENT_TARGET 10.10 CACHE STRING "macOS Deployment Target")
    endif()
endif()

project(pdm VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D MAC_OS -D LINUX")

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES ../sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES ./wxmac.icns)
endif()


# Put executable in build directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(src)
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            RESOURCE ".wxmac.icns"
            MACOSX_BUNDLE_ICON_FILE wxmac.icns
            MACOSX_BUNDLE_COPYRIGHT "Copyright 2021 Yi Yang"
            MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxwidgets.YiYang"
            )
endif()
