#pragma once
#include <string_view>

namespace oeng::graphics
{
	class Shader
	{
	public:
		Shader(std::string_view vert_name, std::string_view frag_name);
		~Shader();

	private:
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
	};
}
