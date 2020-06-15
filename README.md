# Otherengine

3D Game Engine made with C++ and OpenGL

[Engine usage example](https://github.com/Othereum/Otherengine-TestGame)

* **C++20**, OpenGL 3.3, SDL2

* Cross platform: Windows, OS X, Linux

* Math library [**otm**](https://github.com/Othereum/otm)

* Memory pool library [**omem**](https://github.com/Othereum/omem)

* 3D Rendering
  * Support custom GLSL shader
  * 3D Mesh (custom format .omesh)
  * Texture (jpg, png, ...)

* Game Framework
  * UE4 style
  * Actor - Component
  * Hierarchical SceneComponent
  * World timer manager
  * Multiplayer support (planned)

* Modules and dependencies (*italics* means private)
  * [**Core**](https://github.com/Othereum/Otherengine/tree/buildable/Source/Runtime/Core) - [nlohmann-json](https://github.com/nlohmann/json) [fmt](https://github.com/fmtlib/fmt) [spdlog](https://github.com/gabime/spdlog)
  * [**Engine**](https://github.com/Othereum/Otherengine/tree/buildable/Source/Runtime/Engine) - Core Renderer [*SDL2*](https://www.libsdl.org/download-2.0.php)
  * [**Renderer**](https://github.com/Othereum/Otherengine/tree/buildable/Source/Runtime/Renderer) - Core [*SDL2*](https://www.libsdl.org/download-2.0.php) [*GLEW*](https://github.com/nigels-com/glew) [*SOIL2*](https://github.com/SpartanJ/SOIL2)
