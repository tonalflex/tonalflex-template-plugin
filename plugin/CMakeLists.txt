cmake_minimum_required(VERSION 3.22.1)

# Required by JUCE
set(PLUGIN_NAME "Tonalflex Plugin")
project(plugin VERSION 0.0.1) 

option(BUILD_DESKTOP "Also build Standalone" OFF)
set(_plugin_formats "VST3") # Default plugin format

# Build for Desktop or Embedded (ElkOS)
if (BUILD_DESKTOP)
    set(JUCE_WEB_BROWSER_VALUE 1)               # Webview UI for Desktop
    list(APPEND _plugin_formats "Standalone")   # Add Standalone format
    set(VST3_MANIFEST TRUE)                     # Create moduleinfo.json for VST3
     # Find GTK and WebKitGTK for Linux desktop builds
    if (UNIX AND NOT APPLE) # Linux-specific
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GTK3 REQUIRED gtk+-3.0)           # GTK 3
        pkg_check_modules(WEBKITGTK REQUIRED webkit2gtk-4.1) # WebKitGTK
    endif()
else()
    set(JUCE_WEB_BROWSER_VALUE 0)               # No Webview UI for Embedded
    find_package(JUCE CONFIG REQUIRED)           # Use ElkOS SDK provided JUCE
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF) # Disable LTO
    set(VST3_MANIFEST FALSE)                    # Required for ElkOS cross-compilation
endif()

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME TonalFlex
    PLUGIN_NAME ${PLUGIN_NAME}
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE MCMP
    PLUGIN_CODE EXPL
    FORMATS ${_plugin_formats}
    PRODUCT_NAME ${PROJECT_NAME}
    VST3_AUTO_MANIFEST ${VST3_MANIFEST}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        include/plugin_editor.h
        include/plugin_processor.h
        src/plugin_editor.cpp
        src/plugin_processor.cpp
        
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# JUCE modules used
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_devices

        # Additional modules
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra

        # Conditional libraries for Linux x86_64
        # Linux-specific libraries (only for desktop builds)
        $<$<AND:$<BOOL:${BUILD_DESKTOP}>,$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${GTK3_LIBRARIES}>
        $<$<AND:$<BOOL:${BUILD_DESKTOP}>,$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${WEBKITGTK_LIBRARIES}>

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
        # juce::juce_recommended_lto_flags # Must be disabled for ElkOS build
)

# Include directories for GTK/WebKitGTK on Linux
if (BUILD_DESKTOP AND UNIX AND NOT APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKITGTK_INCLUDE_DIRS})
endif()

# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=${JUCE_WEB_BROWSER_VALUE}
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Treat all warnings as errors
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Visual Studio Only
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..) 