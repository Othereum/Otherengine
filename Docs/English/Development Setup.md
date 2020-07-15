# Development Setup

## Prerequisites

* cmake
* C++20 compiler
* [Dependencies](https://github.com/Othereum/Otherengine#dependencies): You can use [vcpkg](https://github.com/microsoft/vcpkg) for Windows.

## Get Started

1. First, create a game project folder.

    The folder name doesn't matter, but you'd better use the game name. In this tutorial, we'll go with TestGame.

    ```text
    ◢ TestGame
    ```

1. Add the Otherengine to the Engine folder.

    In this tutorial, we will use the git submodule. This will facilitate engine updates.

    ```properties
    # .gitmodules
    [submodule "Engine"]
        path = Engine
        url = https://github.com/Othereum/Otherengine.git
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

1. Add CMakeLists.txt.

    ```text
    ◢ TestGame
      ▷ Engine
        .gitmodules
        CMakeLists.txt
    ```

    ```cmake
    cmake_minimum_required(VERSION 3.8)
    project("TestGame")
    add_subdirectory("Engine")
    ```

    You just need to write these three lines. The CMakeLists.txt inside the engine will take care of the entire build configuration.

1. Create a Public and Private folder in TestGame/Source/TestGame.

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

1. Create a cpp file to create a game module.

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

1. Generate with CMake and build them all.

    In Windows, this is what the build result will look like:

    ```text
    ◢ TestGame
      ◢ Binaries
          omem.dll
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
    ```

    All build results are stored in Binaries. Try running TestGame.exe. If it works well, you'll see a black screen because we haven't added anything yet.

That's it! So we finished the development setup. Now we can begin to develop.
