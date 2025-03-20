#!/bin/bash

set -e  # Stop on errors

cd /home/yoctouser/plugin
mkdir -p build && cd build

# Ensure the toolchain is sourced
if [ -f /SDKs/elkpi/environment-setup-cortexa72-elk-linux ]; then
    echo "🔧 Sourcing Elk Audio toolchain..."
    source /SDKs/elkpi/environment-setup-cortexa72-elk-linux
else
    echo "❌ Error: Elk Audio toolchain not found!" >&2
    exit 1
fi

# Clean and reconfigure CMake
rm -rf CMakeCache.txt CMakeFiles/

# Configure and build the plugin
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Copy the plugin to the bin directory (on the host)
mkdir -p /home/yoctouser/bin
cp -r ./plugin_artefacts/Release/VST3/plugin.vst3 /home/yoctouser/bin/
echo "✅ Plugin copied to /bin/plugin.vst3"

# Remove the temporary build directory
rm -rf /home/yoctouser/plugin/build
