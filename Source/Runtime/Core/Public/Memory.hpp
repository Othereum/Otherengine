#pragma once
#include <omem.hpp>
#include "Core.hpp"
#include "Log.hpp"

namespace logcat
{
	extern CORE_API const LogCategory kMemory;
}

namespace oeng::core
{
	/**
	 * Initialize memory pool for current thread.
	 * It must be called only once before using memory pool.
	 */
	CORE_API void InitMemPool();

	/**
	 * Get memory pool for current thread.
	 * Must initialize it via InitMemPool() before using it.
	 */
	[[nodiscard]] CORE_API omem::MemoryPoolManager& GetMemPool() noexcept;

	/**
	 * Clean up and log info of memory pool for current thread.
	 * It must be called only once after using memory pool.
	 * You really should not use memory pool after calling it.
	 */
	CORE_API void CleanUpMemPool() noexcept;

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
