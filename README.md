# TireCentral

**TireCentral** is an interactive desktop application for visualizing and configuring various tire models used in vehicle dynamics simulations. Built with modern C++ and featuring real-time plotting capabilities.

## Features

- **Multiple Tire Models**: Support for 7 different tire models
  - Simple Pacejka
  - Magic Formula (MF 6.2, MC Tyre 1.1, MF 5.2, MF 6.0)
  - Classic Pacejka (1989, 1994)
  - Fiala
  - UA (University of Arizona)
  - Dugoff
  - Tire 521

- **Interactive Visualization**
  - Real-time parameter adjustment with step controls
  - Pure and combined force plots (Fx, Fy)
  - Moment visualization (Mx - Overturning, My - Rolling Resistance, Mz - Aligning Torque)
  - Force envelope (friction circle) comparison
  - Fz variation analysis with multiple delta curves

- **Configuration Management**
  - Save/load tire configurations in JSON format
  - Support for different feature levels per model
  - Comprehensive parameter sets for each model

## Dependencies

All dependencies are bundled in the `ThirdParty` directory to ensure stability and prevent breaking changes from updates:

- **Dear ImGui** - Immediate mode GUI library
- **ImPlot** - Plotting library for ImGui
- **GLFW** - Window management and OpenGL context creation
- **tinyfiledialogs** - Native file dialog support
- **OpenGL** - Graphics rendering

## Building

### Prerequisites

- CMake 3.8 or higher
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- OpenGL 3.0+ support
