#pragma once
#include <memory>

namespace oeng
{
	[[nodiscard]] inline void* Alloc(size_t n)
	{
		return operator new(n);
	}

	inline void Free(void* p) noexcept
	{
		operator delete(p);
	}
	
	template <class T, class... Args>
	[[nodiscard]] T* New(Args&&... args)
	{
		return new T{std::forward<Args>(args)...};
	}

	template <class T, class... Args>
	[[nodiscard]] T* NewArr(size_t n, Args&&... args)
	{
		return new T[n]{std::forward<Args>(args)...};
	}

	template <class T>
	void Delete(T* p) noexcept
	{
		delete p;
	}

	template <class T>
	void DeleteArr(T* p) noexcept
	{
		delete[] p;
	}

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