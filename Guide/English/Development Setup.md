# Development Setup

[한국어](../한국어/개발%20셋업.md)

## Install Engine

Install the engine in the desired position.

```shell
$ git clone -b buildable --recurse-submodules https://github.com/Othereum/Otherengine
```

## Game Project Configuration

Template helps you setup your game projects quickly and easily: <https://github.com/Othereum/Otherengine-Template>

## Installing Dependencies

Item | Min Version | Auto Install
--- | --- | ---
MSVC\* | 14.26 | X
Clang\* | 10 | `Generate.sh` - apt
GCC\* | 10 | `Generate.sh` - apt
[CMake](https://cmake.org/download) | 3.14 | `InstallDeps.sh` - wget

\* *Only need one of three.*

Item | Min Version | Auto Install
--- | --- | ---
[{fmt}](https://github.com/fmtlib/fmt) | 7.0.0 | CMakeLists.txt - git
[spdlog](https://github.com/gabime/spdlog) | 1.7.0 | CMakeLists.txt - git
[nlohmann-json](https://github.com/nlohmann/json) | 3.8.0 | CMakeLists.txt - git
[SDL2](https://www.libsdl.org/download-2.0.php) | | `InstallDeps.sh` - apt
[GLEW](https://github.com/nigels-com/glew) | | `InstallDeps.sh` - apt
[SOIL2](https://github.com/SpartanJ/SOIL2) | | CMakeLists.txt - git
[omem](https://github.com/Othereum/omem) | | CMakeLists.txt - git
[otm](https://github.com/Othereum/otm) | | CMakeLists.txt - git

- Linux

  Enter following command in shell:

  ```shell
  MyGame$ path_to_engine/Scripts/InstallDeps.sh
  ```

- Windows

  Scripts are not available in Windows. Use [vcpkg](https://github.com/microsoft/vcpkg) to install the library.

  > **NOTE**: The default triplet for vcpkg is x86-window. To install the x64 package, you must specify it.  
  > ex) `vcpkg install sdl2:x64-windows glew:x64-windows`
  >
  > You can also change the default triplet. You can set the value of the system environment variable `VCPKG_DEFAULT_TRIPLET` to `x64-window`.
  >
  > **NOTE 2**: In Visual Stduio 2019 16.6.5, If vcpkg is installed in `Program Files`, there is a bug that completely deletes and regenerates the cache, mistaking that the toolchain has changed each time the CMake cache is created. It is recommended to install vcpkg at `%USERPROFILE%`.

> **Tip**: If possible, install libraries manually. CMakeLists.txt auto-installation slows build speed.

## Generate and Build

- Linux

  - Generate

    ```shell
    MyGame$ path_to_engine/Scripts/Generate.sh [compiler] [cmake_args...]
    ```

    - `[compiler]`: Specifies the compiler to use. You can specify gcc or clang. The default is clang.
    - `[cmake_args...]`: Additional command-line options passed to CMake.

  - Build

    ```shell
    MyGame$ path_to_engine/Scripts/Build.sh [compiler] [cmake_args] [build_args]
    ```

    - `[compiler]`: Specifies the compiler to use.
    - `[cmake_args]`: Additional command-line options passed to CMake. If multiple, enclose in quotation marks.
    - `[build_args]`: Specifies the native build switch that passes after --. If multiple, enclose in quotation marks.

- Windows

  Visual Studio makes building easy and simple. Check out [CMake projects in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019) for more information.

In Windows, this is what the build result will look like:

```text
◢ MyGame
  ◢ Binaries
      Otherengine-Core.dll
      Othrengine-Engine.dll
      Otherengine-Renderer.dll
      MyGame.exe
      MyGame-Module.dll
      ...
  ▷ out
  ▷ Source
    CMakeLists.txt
    CMakeSettings.json
    ...
```

All build results are stored in Binaries. Try running TestGame.exe. If it works well, you'll see a black screen because we haven't added anything yet.

That's it! So we finished the development setup. Now we can begin to develop.
