#pragma once
#include <stdexcept>
#include <GL/glew.h>
#include "Log.hpp"
#include "Format.hpp"

namespace oeng
{
	class OpenGlError : public std::runtime_error
	{
	public:
		explicit OpenGlError(unsigned code)
			:std::runtime_error{format("OpenGL error occured (code: {})", code)}, code_{code}
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

	inline void GlWarnError()
	{
		if (const auto err = glGetError()) log::Warn("OpenGL error occured (code: {})", err);
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
	decltype(auto) gl(std::nothrow_t, Fn fn, Args&&... args) noexcept
	{
		using R = std::invoke_result_t<Fn, Args...>;
		if constexpr (std::is_void_v<R>)
		{
			fn(std::forward<Args>(args)...);
			GlWarnError();
		}
		else
		{
			R ret = fn(std::forward<Args>(args)...);
			GlWarnError();
			return static_cast<R>(ret);
		}
	}
}
