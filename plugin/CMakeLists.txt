cmake_minimum_required(VERSION 3.22.1)

# Required by JUCE (Change these to project specs)
set(PLUGIN_NAME "Plugin")            # Name of the plugin (used in the UI)
set(PLUGIN_CODE TFTP)                # Unique 4-letter code for the plugin
project(plugin VERSION 0.0.1)        # Set version of the plugin

option(HEADLESS "Disable GUI (headless/embedded build)" ON)
set(_plugin_formats "VST3") # Default plugin format

# Build for Desktop or Embedded (ElkOS)
if (NOT HEADLESS)
    set(JUCE_WEB_BROWSER_VALUE 1)               # Webview UI for Desktop
    list(APPEND _plugin_formats "Standalone")   # Add Standalone format
    # list(APPEND _plugin_formats "AU")           # Add AU (Apple) format
    # list(APPEND _plugin_formats "AAX")          # Add AAX (Avid) format
    set(VST3_MANIFEST TRUE)                     # Create moduleinfo.json for VST3
    add_compile_definitions(HEADLESS=0)
else()
    set(JUCE_WEB_BROWSER_VALUE 0)               # No Webview UI for Embedded
    find_package(JUCE CONFIG REQUIRED)           # Use ElkOS SDK provided JUCE
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF) # Disable LTO
    set(VST3_MANIFEST FALSE)                    # Required for ElkOS cross-compilation
    add_compile_definitions(HEADLESS=1)
endif()

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME TonalFlex
    PLUGIN_NAME ${PLUGIN_NAME}
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE TFTF
    PLUGIN_CODE ${PLUGIN_CODE}
    FORMATS ${_plugin_formats}
    PRODUCT_NAME ${PROJECT_NAME}
    VST3_AUTO_MANIFEST ${VST3_MANIFEST}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        include/plugin_processor.h
        src/plugin_processor.cpp
)
# Include GUI for Desktop builds
if (NOT HEADLESS)
    target_sources(${PROJECT_NAME}
        PRIVATE
            include/plugin_editor.h
            src/plugin_editor.cpp
    )
endif()

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

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
        # juce::juce_recommended_lto_flags # Must be disabled for ElkOS build
)

if (NOT HEADLESS)
    # Add Webview GUI files as binary data
    file(GLOB_RECURSE WEB_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/webview/*.*")

    juce_add_binary_data(WebAssets
    SOURCES ${WEB_ASSETS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE WebAssets)
    target_include_directories(${PROJECT_NAME}
        PRIVATE ${WebAssets_BINARY_DIR}
    )
else()
    # Generate a dummy BinaryData.h to avoid missing include errors
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/BinaryData.h" "// Empty stub for Embedded builds\n")
    target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )
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