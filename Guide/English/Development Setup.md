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

### Build Toolchain

Item | Min Version | Note
--- | --- | ---
MSVC | 19.26 | Windows
Clang | 10 | Linux
[CMake](https://cmake.org/download) | 3.16 | \*

\* For Linux, we do provide a auto installation script: `$ ./GetCMake.sh [version]`; However, it is recommended to install it using package manager.

### Libraries

Item | Min Version | Auto Install
--- | --- | ---
[{fmt}](https://github.com/fmtlib/fmt) | 7.0.0 | O
[spdlog](https://github.com/gabime/spdlog) | 1.7.0 | O
[nlohmann-json](https://github.com/nlohmann/json) | 3.9.0 | O
[FMOD] | 2.00 | △\*
[SDL2](https://www.libsdl.org/download-2.0.php) | | X
[GLEW](https://github.com/nigels-com/glew) | | X
[SOIL2](https://github.com/SpartanJ/SOIL2) | | O
[omem](https://github.com/Othereum/omem) | | O
[otm](https://github.com/Othereum/otm) | | O

[FMOD]: https://www.fmod.com

\* You have to sign up on the [FMOD official website][FMOD]. In Windows, you can download the installer and install it in the default path(`C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows`). On Linux, install manually on `/usr/local/` (recommended) or use a script: `$ ./GetFMOD.sh <username> <password> [version]`

Use [vcpkg](https://github.com/microsoft/vcpkg) to install libraries manually in Windows.

> **NOTE**: The default triplet for vcpkg is `x86-window`. To install the x64 package, you must specify it.  
> ex) `vcpkg install sdl2:x64-windows glew:x64-windows`
>
> You can also change the default triplet. You can set the value of the system environment variable `VCPKG_DEFAULT_TRIPLET` to `x64-window`.

> **NOTE 2**: In Visual Stduio 2019 16.6.5, If vcpkg is installed in `Program Files`, there is a bug that completely deletes and regenerates the cache, mistaking that the toolchain has changed each time the CMake cache is created. It is recommended to install vcpkg at `C:`.

> **Tip**: If possible, install libraries manually. Auto-installation slows build speed.

## Build

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

All build results are stored in Binaries. Try running MyGame.exe. If it works well, you'll see a black screen because we haven't added anything yet.

That's it! So we finished the development setup. Now we can begin to develop.
