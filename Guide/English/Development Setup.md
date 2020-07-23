# Development Setup

[한국어](../한국어/개발%20셋업.md)

## Game Project Configuration

### Create a game project folder

The folder name doesn't matter, but you'd better use the game name. In this tutorial, we'll go with TestGame.

```text
◢ TestGame
```

### Add Otherengine

Add the Otherengine to the Engine folder. In this tutorial, we will use the git submodule. This will facilitate engine updates.

```shell
TestGame$ git init
TestGame$ git submodule add -b buildable https://github.com/Othereum/Otherengine Engine --depth 1
```

```text
◢ TestGame
  ◢ Engine
    ▷ Config
    ▷ Docs
    ▷ Shader
    ▷ Source
      ...
    .gitmodules
```

### Write CMakeLists.txt

```text
◢ TestGame
  ▷ Engine
    .gitmodules
    CMakeLists.txt
```

```cmake
cmake_minimum_required(VERSION 3.14)
project("TestGame")
add_subdirectory("Engine")
```

You just need to write these three lines. The CMakeLists.txt inside the engine will take care of the entire build configuration.

### Configure Module

Create a Public and Private folder in TestGame/Source/TestGame.

```text
◢ TestGame
  ▷ Engine
  ◢ Source
    ◢ TestGame
      ◢ Public
      ◢ Private
    .gitmodules
    CMakeLists.txt
```

A folder in the Source directory means a module. A folder with the same name as the project name is automatically added to the build target as game module. In this tutorial, it's TestGame. You can add custom modules if you like, but they are not automatically added to the build target, so you must add them manually to CMakeLists.txt.

Each module's Private folder contains sources that will only be used inside the module, and the Public folder contains that will be visible from outside the module. So usually .cpp files are put into Private and .hpp files into Public. All .cpp files in the Private are automatically added to the build target, and the Public folder is added to the include directory for modules that use it.

### Implement Module

Create a cpp file to create a game module.

```text
◢ TestGame
  ▷ Engine
  ◢ Source
    ◢ TestGame
      ◢ Public
      ◢ Private
          TestGame.cpp
    .gitmodules
    CMakeLists.txt
```

```cpp
#include <Engine.hpp>

using namespace oeng;

DEFINE_GAME_MODULE(u8"Test Game");

GAME_API void GameMain(Engine& e)
{
}
```

## Installing Dependencies

Item | Min Version | Auto Install
--- | --- | ---
MSVC\* | 14.26 | X
Clang\* | 10 | `Generate.sh` - `apt`
GCC\* | 10 | `Generate.sh` - `apt`
[CMake](https://cmake.org/download) | 3.14 | `InstallDeps.sh` - `wget`

\* *Only need one of three.*

Item | Min Version | Auto Install
--- | --- | ---
[{fmt}](https://github.com/fmtlib/fmt) | 7.0.0 | CMakeLists.txt - git
[spdlog](https://github.com/gabime/spdlog) | 1.7.0 | CMakeLists.txt - git
[nlohmann-json](https://github.com/nlohmann/json) | 3.8.0 | CMakeLists.txt - git
[SDL2](https://www.libsdl.org/download-2.0.php) | | `InstallDeps.sh` - `apt`
[GLEW](https://github.com/nigels-com/glew) | | `InstallDeps.sh` - `apt`
[SOIL2](https://github.com/SpartanJ/SOIL2) | | CMakeLists.txt - git
[omem](https://github.com/Othereum/omem) | | CMakeLists.txt - git
[otm](https://github.com/Othereum/otm) | | CMakeLists.txt - git

- Linux

  Enter following command in shell:

  ```shell
  TestGame$ Engine/Scripts/InstallDeps.sh
  ```

- Windows

  Scripts are not available in Windows. Use [vcpkg](https://github.com/microsoft/vcpkg) to install the library.

  > **NOTE**: The default triplet for vcpkg is x86-window. To install the x64 package, you must specify it.
  > 예) `vcpkg install sdl2:x64-windows glew:x64-windows`
  >
  > You can also change the default triplet. You can set the value of the system environment variable `VCPKG_DEFAULT_TRIPLET` to `x64-window`.

> **Tip**: If possible, install it manually. CMakeLists.txt auto-installation slows build speed.

## Generate and Build

- Linux

  - Generate

    ```shell
    TestGame$ Engine/Scripts/Generate.sh [compiler] [cmake_args...]
    ```

    - `[compiler]`: Specifies the compiler to use. You can specify gcc or clang. The default is clang.
    - `[cmake_args...]`: Additional command-line options passed to CMake.

  - Build

    ```shell
    TestGame$ Engine/Scripts/Build.sh [compiler] [cmake_args] [build_args]
    ```

    - `[compiler]`: Specifies the compiler to use.
    - `[cmake_args]`: Additional command-line options passed to CMake. If multiple, enclose in quotation marks.
    - `[build_args]`: Specifies the native build switch that passes after --. If multiple, enclose in quotation marks.

- Windows

  Visual Studio makes building easy and simple. Check out [CMake projects in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019) for more information.

In Windows, this is what the build result will look like:

```text
◢ TestGame
  ◢ Binaries
      Otherengine-Core.dll
      Othrengine-Engine.dll
      Otherengine-Renderer.dll
      TestGame.exe
      TestGame-Module.dll
      ...
  ▷ Engine
  ▷ out
  ▷ Source
    .gitmodules
    CMakeLists.txt
    CMakeSettings.json
    ...
```

All build results are stored in Binaries. Try running TestGame.exe. If it works well, you'll see a black screen because we haven't added anything yet.

That's it! So we finished the development setup. Now we can begin to develop.
