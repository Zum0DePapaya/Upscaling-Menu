# Upscaling Menu for Unreal Engine

A free, drag-and-drop UMG plugin that provides a complete, intelligent graphics upscaling menu. It automatically detects the user's GPU and only enables the upscaling options that are actually supported by their hardware.

## Features

- **Automatic GPU Detection**: A lightweight C++ helper function identifies the user's GPU vendor (NVIDIA, AMD, Intel) and model series.
- **Dynamic UI**: The menu intelligently enables or disables options for DLSS, FSR, XeSS, and NIS based on real-time hardware and software support checks.
- **Self-Contained**: Contains its own logic for hardware and software detection and does not require any third-party plugins to function.
- **Easy Integration**: Implemented as a single UMG Widget, that can be dropped into any existing UI or added directly to the viewport.

## Plugins

This plugin is a UI and management tool. It **DOES NOT** contain or redistribute the technologies themselves. If you install no other plugins only the "Stretch" option will work. Download, install and enable whichever technologies you want to use in your project.

- **NVIDIA DLSS + StreamlineCore + StreamlineDLSSG + StreamlineReflex + NIS**: http://developer.nvidia.com/rtx/dlss
- **AMD FidelityFX FSR 4**: http://gpuopen.com/learn/ue-fsr4
- **AMD Radeon Anti-Lag 2**: https://gpuopen.com/anti-lag-2
- **Intel XeSS**: http://github.com/GameTechDev/XeSSUnrealPlugin
