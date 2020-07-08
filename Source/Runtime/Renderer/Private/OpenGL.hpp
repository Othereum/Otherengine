#pragma once
#include <stdexcept>
#include <GL/glew.h>
#include "Debug.hpp"
#include "Format.hpp"

namespace oeng
{
	class OpenGlError : public std::runtime_error
	{
	public:
		explicit OpenGlError(unsigned code)
			:std::runtime_error{fmt::format("OpenGL error occured (code: {})", code)}, code_{code}
		{
		}

		[[nodiscard]] unsigned GetCode() const noexcept { return code_; }

	private:
		unsigned code_;
	};

	inline void GlCheckError()
	{
		if (const auto err = glGetError()) throw OpenGlError{err};
	}

	template <class... Args, std::invocable<Args...> Fn>
	decltype(auto) gl(Fn fn, Args&&... args)
	{
		using R = std::invoke_result_t<Fn, Args...>;
		if constexpr (std::is_void_v<R>)
		{
			fn(std::forward<Args>(args)...);
			GlCheckError();
		}
		else
		{
			R ret = fn(std::forward<Args>(args)...);
			GlCheckError();
			return static_cast<R>(ret);
		}
	}

	template <class... Args, std::invocable<Args...> Fn>
	decltype(auto) gl(unsigned& err, Fn fn, Args&&... args) noexcept
	{
		using R = std::invoke_result_t<Fn, Args...>;
		if constexpr (std::is_void_v<R>)
		{
			fn(std::forward<Args>(args)...);
			err = glGetError();
		}
		else
		{
			R ret = fn(std::forward<Args>(args)...);
			err = glGetError();
			return static_cast<R>(ret);
		}
	}
}

#define GL(fn, ...) [&]{ unsigned err; ::oeng::gl(err, fn, ##__VA_ARGS__); EXPECT_MSG(err == GL_NO_ERROR, u8"{}", err); }()
