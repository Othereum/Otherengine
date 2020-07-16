#pragma once

struct SDL_Window;

namespace oeng::core
{
	class IEngine;
}

namespace oeng::renderer
{
	class Renderer;
	
	class Material;
	class Texture;
	class Mesh;
	class Shader;
	
	class VertexArray;
	struct Vertex;
	
	class IMeshComponent;
	class ISpriteComponent;
	class ICamera;
	
	class IDirLight;
	class ISkyLight;
	class IPointLight;
	class ISpotLight;
}

namespace oeng
{
	using namespace renderer;
}
