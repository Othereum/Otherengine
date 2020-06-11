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

	inline unsigned GlLogError()
	{
		const auto err = glGetError();
		if (err) log::Error("OpenGL error occured (code: {})", err);
		return err;
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
			err = GlLogError();
		}
		else
		{
			R ret = fn(std::forward<Args>(args)...);
			err = GlLogError();
			return static_cast<R>(ret);
		}
	}
}
