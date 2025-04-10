#!/bin/bash

set -e  # Stop on errors

cd /home/yoctouser/plugin
mkdir -p build && cd build

# Ensure the toolchain is sourced
if [ -f /SDKs/elkpi/environment-setup-cortexa72-elk-linux ]; then
    echo "🔧 Sourcing ElkOS SDK..."
    source /SDKs/elkpi/environment-setup-cortexa72-elk-linux
else
    echo "❌ Error: ElkOS toolchain not found!" >&2
    exit 1
fi

# Configure and build the plugin
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Zip the VST3 plugin to the bin directory
PLUGIN_PATH="./plugin_artefacts/Release/VST3"
CONFIG_FILE="/home/yoctouser/plugin/sushi-config.json"
TAR_FILE="/home/yoctouser/bin/plugin-arm64-headless.tar.gz"

mkdir -p /home/yoctouser/bin
chmod 777 /home/yoctouser/bin

[[ -d "$PLUGIN_PATH" ]] || { echo "❌ Error: VST3 plugin not found at $PLUGIN_PATH"; exit 1; }
[[ -f "$CONFIG_FILE" ]] || { echo "❌ Error: Sushi-config.json not found at $CONFIG_FILE"; exit 1; }
[[ -f "$TAR_FILE" ]] && rm -f "$TAR_FILE"

echo "📦 Creating tar.gz of VST3 plugin and configuration file..."
cp "$CONFIG_FILE" "$PLUGIN_PATH/"
tar -czf "$TAR_FILE" -C "$PLUGIN_PATH" .
echo "✅ Plugin & Sushi JSON config packaged: $TAR_FILE"

# Remove the temporary build directory
rm -rf /home/yoctouser/plugin/build
