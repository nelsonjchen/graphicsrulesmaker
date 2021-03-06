cmake_minimum_required(VERSION 3.1.0)
project(GraphicsRulesMaker)

# Find other dependencies
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

if(WIN32)
    find_package(DirectX REQUIRED)
    include_directories(${DirectX_INCLUDE_DIR})

    if(DEFINED DirectX_DXGI_LIBRARY)
        message(STATUS "DXGI Library found at " ${DirectX_DXGI_LIBRARY})
        add_definitions(-DDXGI)
    endif(DEFINED DirectX_DXGI_LIBRARY)
endif(WIN32)

if (NOT DEFINED GraphicsRulesMaker_TRANSLATIONS_DIR)
    # This will put only this subproject's translations in a subdir within this project
    set(GraphicsRulesMaker_TRANSLATIONS_DIR "${PROJECT_SOURCE_DIR}/translations")
endif (NOT DEFINED GraphicsRulesMaker_TRANSLATIONS_DIR)

set(GraphicsRulesMaker_SRCS
    videocarddatabase.cpp
    devicemodel.cpp
)

set(GraphicsRulesMaker_HDRS
    "devicemodel.h"
    "videocarddatabase.h"
    "gamewriterinterface.h"
    "${PROJECT_BINARY_DIR}/graphicsrulesmaker_config.h"
)

# Translations
if(${Qt5LinguistTools_FOUND})
    message(STATUS "Qt Linguist found - will set up translations")
    qt5_create_translation(GraphicsRulesMaker_QMS
        ${CMAKE_CURRENT_SOURCE_DIR}
        "${GraphicsRulesMaker_TRANSLATIONS_DIR}/GraphicsRulesMaker_nl.ts"
        OPTIONS -source-language "en_US"
    )
endif(${Qt5LinguistTools_FOUND})

# Determine installation path for CMake files
if(WIN32 AND NOT CYGWIN)
    set(INSTALL_CMAKE_DIR CMake)
else()
    set(INSTALL_CMAKE_DIR lib/cmake/GraphicsRulesMaker)
endif()

# Determine plugin installation path
# A bit of duplicate code (shared with the main project) to support split builds
if(NOT DEFINED GraphicsRulesMaker_PLUGIN_PATH)
    if(WIN32 AND NOT CYGWIN)
        set(PLUGIN_INSTALL_DIR bin/gamewriters/)
    else(WIN32 AND NOT CYGWIN)
        set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (eg. '64')")
        set(LIB_INSTALL_DIR lib${LIB_SUFFIX} )
        set(PLUGIN_INSTALL_DIR lib${LIB_SUFFIX}/graphicsrulesmaker/)
    endif(WIN32 AND NOT CYGWIN)
    set(GraphicsRulesMaker_PLUGIN_PATH ${CMAKE_INSTALL_PREFIX}/${PLUGIN_INSTALL_DIR} )
endif(NOT DEFINED GraphicsRulesMaker_PLUGIN_PATH)
# And for .qm files
if(NOT DEFINED GraphicsRulesMaker_TRANSLATIONS_PATH)
    if(WIN32 AND NOT CYGWIN)
        set(TRANSLATIONS_INSTALL_DIR share/)
    else(WIN32 AND NOT CYGWIN)
        set(TRANSLATIONS_INSTALL_DIR share/graphicsrulesmaker/)
    endif(WIN32 AND NOT CYGWIN)
    set(GraphicsRulesMaker_TRANSLATIONS_PATH ${CMAKE_INSTALL_PREFIX}/${TRANSLATIONS_INSTALL_DIR} )
endif(NOT DEFINED GraphicsRulesMaker_TRANSLATIONS_PATH)

# Version information
set(GraphicsRulesMaker_MAJOR_VERSION 1)
set(GraphicsRulesMaker_MINOR_VERSION 90)
set(GraphicsRulesMaker_PATCH_VERSION 0)
set(GraphicsRulesMaker_VERSION
  ${GraphicsRulesMaker_MAJOR_VERSION}.${GraphicsRulesMaker_MINOR_VERSION}.${GraphicsRulesMaker_PATCH_VERSION})

# Configuration file
configure_file(graphicsrulesmaker_config.h.cmake
    ${PROJECT_BINARY_DIR}/graphicsrulesmaker_config.h @ONLY)

add_library(GraphicsRulesMaker SHARED ${GraphicsRulesMaker_SRCS} ${GraphicsRulesMaker_QMS})
target_compile_definitions(GraphicsRulesMaker PRIVATE GRAPHICSRULESMAKER_LIBRARY)
target_link_libraries(GraphicsRulesMaker Qt5::Widgets)
if(WIN32)
    # NOTE: ${DirectX_DXGI_LIBRARY} is removed; we load it dynamically at runtime
    target_link_libraries(GraphicsRulesMaker ${DirectX_LIBRARY} )
endif(WIN32)

if(WIN32)
    install(TARGETS GraphicsRulesMaker
        EXPORT GraphicsRulesMakerTargets
        RUNTIME DESTINATION bin
        COMPONENT application
    )
    # Different component for installer
    install(TARGETS GraphicsRulesMaker
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
        COMPONENT dev
    )
else(WIN32)
    install(TARGETS GraphicsRulesMaker
        EXPORT GraphicsRulesMakerTargets
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
        COMPONENT application
    )
endif(WIN32)

install(
    FILES ${GraphicsRulesMaker_HDRS}
    DESTINATION include/graphicsrulesmaker
    COMPONENT dev
)
install(
    FILES ${GraphicsRulesMaker_QMS}
    DESTINATION ${TRANSLATIONS_INSTALL_DIR}
    COMPONENT application
)

# Write out CMake files (for developers)
export(TARGETS GraphicsRulesMaker
    FILE "${PROJECT_BINARY_DIR}/GraphicsRulesMakerTargets.cmake")
# For installation (using absolute path on non-windows)
if(WIN32 AND NOT CYGWIN)
    set(CONF_INCLUDE_DIRS "\${GraphicsRulesMaker_CMAKE_DIR}/../include")
    set(GraphicsRulesMaker_EXPORT_PLUGIN_PATH "\${GraphicsRulesMaker_CMAKE_DIR}/../${PLUGIN_INSTALL_DIR}")
else(WIN32 AND NOT CYGWIN)
    set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
    set(GraphicsRulesMaker_EXPORT_PLUGIN_PATH "${GraphicsRulesMaker_PLUGIN_PATH}")
endif(WIN32 AND NOT CYGWIN)

configure_file(GraphicsRulesMakerConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GraphicsRulesMakerConfig.cmake" @ONLY)
# Version info
configure_file(GraphicsRulesMakerVersion.cmake.in
  "${PROJECT_BINARY_DIR}/GraphicsRulesMakerVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GraphicsRulesMakerConfig.cmake"
  "${PROJECT_BINARY_DIR}/GraphicsRulesMakerVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
  COMPONENT dev
)

install(EXPORT GraphicsRulesMakerTargets
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev
)
