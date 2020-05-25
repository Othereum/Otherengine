#pragma once
#include <utility>

namespace oeng
{
	template <class T>
	class Wrapper
	{
	public:
		template <class... Args>
		explicit(sizeof...(Args) == 1)
		Wrapper(Args&&... args) :object_{std::forward<Args>(args)...} {}

		T* operator->() noexcept { return &object_; }
		const T* operator->() const noexcept { return &object_; }

		[[nodiscard]] T* Get() noexcept { return &object_; }
		[[nodiscard]] const T* Get() const noexcept { return &object_; }

		explicit operator T&() noexcept { return &object_; }
		explicit operator const T&() const noexcept { return &object_; }

	private:
		T object_;
	};
}
