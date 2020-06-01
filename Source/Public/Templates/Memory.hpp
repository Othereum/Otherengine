#pragma once
#include <memory>

namespace oeng
{
	template <class T>
	using Allocator = std::allocator<T>;

	template <class T>
	using DefaultDelete = std::default_delete<T>;

	template <class T, class Deleter = DefaultDelete<T>>
	using UniquePtr = std::unique_ptr<T, Deleter>;

	template <class T>
	using SharedPtr = std::shared_ptr<T>;

	template <class T>
	using WeakPtr = std::weak_ptr<T>;

	template <class T>
	using EnableSharedFromThis = std::enable_shared_from_this<T>;

	template <class T, class... Args>
	UniquePtr<T> MakeUnique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <class T, class... Args>
	SharedPtr<T> MakeShared(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}