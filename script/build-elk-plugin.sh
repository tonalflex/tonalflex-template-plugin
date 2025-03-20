#!/bin/bash

set -e  # Stop on errors

cd /home/yoctouser/plugin
mkdir -p build && cd build

# Ensure the toolchain is sourced
if [ -f /SDKs/elkpi/environment-setup-cortexa72-elk-linux ]; then
    echo "🔧 Sourcing Elk Audio SDK..."
    source /SDKs/elkpi/environment-setup-cortexa72-elk-linux
else
    echo "❌ Error: Elk Audio toolchain not found!" >&2
    exit 1
fi

# Configure and build the plugin
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Zip the VST3 plugin to the output directory
PLUGIN_PATH="./plugin_artefacts/Release/VST3"
CONFIG_FILE="/home/yoctouser/plugin/sushi-config.json"
ZIP_FILE="/home/yoctouser/output/elk-vst3-plugin.zip"

mkdir -p /home/yoctouser/output

[[ -d "$PLUGIN_PATH" ]] || { echo "❌ Error: VST3 plugin not found at $PLUGIN_PATH"; exit 1; }
[[ -f "$CONFIG_FILE" ]] || { echo "❌ Error: sushi-config.json not found at $CONFIG_FILE"; exit 1; }
[[ -f "$ZIP_FILE" ]] && rm -f "$ZIP_FILE"

echo "📦 Zipping VST3 plugin and configuration file..."
sudo cp "$CONFIG_FILE" "$PLUGIN_PATH/"
(cd "$PLUGIN_PATH" && zip -r "$ZIP_FILE" ./*)
echo "✅ ElkOS Plugin & Sushi JSON config zipped: $ZIP_FILE"

# Remove the temporary build directory
rm -rf /home/yoctouser/plugin/build
