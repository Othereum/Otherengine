# Otherengine - 3D Game Engine

[![License](https://img.shields.io/badge/Licence-MIT-blue.svg)](LICENSE) [![Build Status](https://travis-ci.com/Othereum/Otherengine.svg?branch=buildable)](https://travis-ci.com/Othereum/Otherengine) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FOthereum%2FOtherengine)](https://github.com/Othereum/Otherengine)

* [Demonstration](https://www.youtube.com/playlist?list=PLRimg1E-To2HJn1jtpYQXFujjI4dbRUsY)
* [Engine usage example](https://github.com/Othereum/Otherengine-TestGame)
* [Getting Started - Development Setup](Guide/English/Development%20Setup.md)
* [API Reference](https://othereum.github.io/Otherengine)

## Features

* Cross platform - Windows, Linux, OS X

* [otm](https://github.com/Othereum/otm) - Math library. Linear algebra and geometry

* 3D Rendering
  * [GLSL](Guide/English/Shader%20Programming.md)
  * Full dynamic lighting: directional, point and spot light
  * [Material](Guide/English/Graphic%20Assets.md#material): sharable common uniform parameters

* Game Framework
  * UE4 style
  * Actor - Component
  * Hierarchical SceneComponent

* Input system
  * [Input mapping](Guide/English/Input.md)
  * Game controller support

## Branches

* **master** - Reflects live development build. It may not even be compiled.
* **buildable** - Compilable build that works well. But it doesn't mean it's stable.

## Dependencies

Must be the same or newer than the specified version. *Italics* means it can be installed automatically if not found from system.

* MSVC v14.26 or *Clang* 10 or *GCC* 10
* *CMake* 3.14
* [*{fmt}*](https://github.com/fmtlib/fmt) 7.0.0
* [*spdlog*](https://github.com/gabime/spdlog) 1.7.0
* [*nlohmann-json*](https://github.com/nlohmann/json) 3.8.0
* [SDL2](https://www.libsdl.org/download-2.0.php)
* [GLEW](https://github.com/nigels-com/glew)
* [*SOIL2*](https://github.com/SpartanJ/SOIL2)
* [*omem*](https://github.com/Othereum/omem)
* [*otm*](https://github.com/Othereum/otm)
