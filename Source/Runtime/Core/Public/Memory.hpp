#pragma once
#include <omem.hpp>
#include "Core.hpp"

namespace oeng::core
{
	[[nodiscard]] CORE_API omem::MemoryPoolManager& GetMemPool() noexcept;

	[[nodiscard]] inline void* Alloc(size_t size)
	{
		return GetMemPool().Alloc(size);
	}

	inline void Free(void* p, size_t size) noexcept
	{
		GetMemPool().Free(p, size);
	}

	template <class T, class... Args>
	[[nodiscard]] T* New(Args&&... args)
	{
		return GetMemPool().New<T>(std::forward<Args>(args)...);
	}

	template <class T, class... Args>
	[[nodiscard]] T* NewArr(size_t n, Args&&... args)
	{
		return GetMemPool().NewArr<T>(n, std::forward<Args>(args)...);
	}

	template <class T>
	void Delete(T* p) noexcept
	{
		GetMemPool().Delete(p);
	}

	template <class T>
	void DeleteArr(T* p, size_t n) noexcept
	{
		GetMemPool().DeleteArr(p, n);
	}
	
	template <class T>
	class PoolAllocator
	{
	public:
		using value_type = T;
		
		constexpr PoolAllocator() noexcept = default;

		template <class Y>
		constexpr PoolAllocator(const PoolAllocator<Y>&) noexcept {}

		template <class Y>
		constexpr bool operator==(const PoolAllocator<Y>&) const noexcept { return true; }

		[[nodiscard]] T* allocate(size_t n) const
		{
			return static_cast<T*>(GetMemPool().Alloc(sizeof(T) * n));
		}

		void deallocate(T* p, size_t n) const noexcept
		{
			GetMemPool().Free(p, sizeof(T) * n);
		}
	};

	template <class T>
	class PoolDeleter
	{
	public:
		static_assert(!std::is_array_v<T> || std::extent_v<T> != 0,
			"Can't delete array with unknown size");
		
		void operator()(T* p) noexcept
		{
			if constexpr (std::is_array_v<T>)
			{
				DeleteArr(p, std::extent_v<T>);
			}
			else
			{
				Delete(p);
			}
		}
	};
}
