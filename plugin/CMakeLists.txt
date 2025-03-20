cmake_minimum_required(VERSION 3.22.1)

set(PLUGIN_NAME "Tonalflex Plugin")

project(plugin VERSION 0.0.1) # Required by JUCE

option(BUILD_DESKTOP "Also build Standalone" OFF)
set(_plugin_formats "VST3") # Default plugin format

# Build for Desktop or Embedded (ElkOS)
if (BUILD_DESKTOP)
    list(APPEND _plugin_formats "Standalone")
    set(VST3_MANIFEST TRUE) # Create moduleinfo.json for VST3
else()
    find_package(JUCE CONFIG REQUIRED) # Use ElkOS SDK provided JUCE
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF) # Disable LTO
    set(VST3_MANIFEST FALSE) # Required for ElkOS cross-compilation
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

        # Conditional libraries for Linux x86_64
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${ALSA_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${X11_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${CURL_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${FREETYPE_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${FONTCONFIG_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${GL_LIBRARIES}>
        $<$<AND:$<BOOL:${UNIX}>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>>:${WEBKIT_LIBRARIES}>

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
        # juce::juce_recommended_lto_flags # Must be disabled for ElkOS build
)

# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Treat all warnings as errors
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..) # Visual Studio Only