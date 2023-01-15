# godot-environment

A reinforcement learning environment for the Godot game engine, inspired by openai-gym. CMake version 3.24 is now required to build this project, as it provides a fantastic integration of FetchContent with find_package. This means that most dependencies will be downloaded and built as needed, unless they are already installed as a package on your system. Only ZeroMQ must be installed manually, due to its complex build system. I **highly** recommended that you use [vcpkg](https://github.com/microsoft/vcpkg) for installing ZeroMQ, but other package managers are available.

Currently, this project is WIP.

## Dependencies

This library is dependent on my fork of [godot-cpp](https://github.com/errata-c/godot-cpp), [ez-serialize](https://github.com/errata-c/ez-serialize), [ZeroMQ](https://github.com/zeromq/libzmq), and [fmtlib](https://github.com/fmtlib/fmt).

