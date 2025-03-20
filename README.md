# TonalFlex Template Plugin

A **template** for building new **VST3 plugins** for **TonalFlex** using **JUCE**.

---

## **📖 Introduction**

The **TonalFlex Template Plugin** provides a starting point for creating **VST3 plugins** using **JUCE**. This template is designed to **simplify development** and ensure **cross-platform compatibility**, including **ElkOS** and **desktop environments**.

It includes:

- **CMake presets** for standardized builds.
- **Prebuilt Docker image** to handle **ElkOS cross-compilation**.
- **Support for Windows, macOS, and Linux** desktop builds.
- **Example CMake structure** for easy integration with JUCE.

Use this template to **kickstart plugin development** with minimal setup.

---

## **⚡ Prerequisites**

Before you begin, ensure you have the following installed:

✅ **Development Tools**:

- [VS Code](https://code.visualstudio.com/) (Recommended IDE)
- [CMake](https://cmake.org/download/) (Build system)
- [Docker](https://www.docker.com/) (For cross-compilation)

✅ **Additional Dependencies**:

- [JUCE Framework](https://juce.com/) (Automatically fetched via CMake)
- [Git](https://git-scm.com/) (Version control)

💡 **Tip**: Use the **CMake Presets** included in this project for an easy, consistent setup!

---

## **🎛️ Getting Started**

When opening this template for the first time:

1. **Select a CMake Preset** in your IDE.
2. The available presets are defined in [`CMakePresets.json`](./CMakePresets.json).
3. This ensures a **consistent build setup** across different environments.

---

## **🚀 Deploying ElkOS VST3 Release**

To build and release a **VST3 plugin for ElkOS**, push a version tag:

```sh
git tag v0.0.1
git push origin v0.0.1
```

This triggers a CI workflow that automatically cross-compiles the JUCE plugin for ElkOS (aarch64).

The built release is available as a ZIP file under [Releases](https://github.com/tonalflex/tonalflex-template-plugin/releases) on GitHub.

---

## **🔨 Local Build Instructions**

### **Build for ElkOS**

Build the plugin for ElkOS using the prebuilt docker image specified in `docker-compose.yml`.

```sh
docker-compose up
```

Once the compilation completes, a zipped VST3 plugin for ElkOS is accessible in the workspace `output` directory.

_Note! You may need to add exectuable permission to the build script first time._

```
chmod +x ./script/build-elk-plugin.sh
```

### **Build for Desktop**

If you want to build the desktop version in Release mode, use:

```sh
# Option 1 - Debug Mode
cmake -B build -S . -DBUILD_DESKTOP=ON

# Option 2 - Release Mode
cmake -B build -S . -DBUILD_DESKTOP=ON -DCMAKE_BUILD_TYPE=Release

# Build Plugin
cmake --build build
```

_Note! Make sure you have selected a CMake Preset before first build, otherwise you may need to delete the build directory, choose preset and build again!_

### **Run Standalone Build**

After building, run:

```sh
# Debug
cd build/plugin/plugin_artefacts/Debug/Standalone

# Release
cd build/plugin/plugin_artefacts/Release/Standalone
```

```sh
# MacOS
open ./plugin.app

# Windows

# Linux
```

---
