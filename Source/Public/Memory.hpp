#pragma once
#include <vector>
#include "otm/Basic.hpp"

namespace oeng
{
	template <size_t BlockSize>
	class MemoryPool
	{
	public:
		static MemoryPool& Get() noexcept
		{
			thread_local MemoryPool pool;
			return pool;
		}

		template <class T = void>
		T* Allocate(size_t count)
		{
			for (auto& c : containers_)
			{
				if (c.remaining < count) continue;
				
				size_t bytes = 0, cnt = 0;
				for (size_t i = 0; i < c.occupied.size(); ++i)
				{
					if (c.occupied[i]) bytes = 0, cnt = 0;
					else if ((++cnt, bytes += BlockSize) >= count * sizeof T)
					{
						c.occupied[i] = true;
						return reinterpret_cast<T*>(c.blocks.data() + i);
					}
				}
			}
		}
		
		void Deallocate(void* ptr, size_t count)
		{
			// TODO: Implement
		}
		
		MemoryPool(const MemoryPool&) = delete;
		MemoryPool(MemoryPool&&) = delete;
		MemoryPool& operator=(const MemoryPool&) = delete;
		MemoryPool& operator=(MemoryPool&&) = delete;
		
	private:
		struct Block
		{
			Block() noexcept {}
			char block[BlockSize];
		};

		struct BlockContainer
		{
			explicit BlockContainer(size_t count)
				:blocks(count), occupied(count), remaining{count}
			{
			}
			
			std::vector<Block> blocks;
			std::vector<bool> occupied;
			size_t first_available = 0;
			size_t remaining;
		};

		explicit MemoryPool(size_t initial_size)
		{
			blocks_.emplace_back(initial_size);
		}

		std::vector<BlockContainer> containers_;
	};

	template <class T>
	T* Allocate(size_t count)
	{
		constexpr auto block_size = otm::PadToPowerOf2(sizeof T);
		return MemoryPool<block_size>::Get().template Allocate<T>(count);
	}
	
	template <class T>
	void Deallocate(T* ptr, size_t count)
	{
		constexpr auto block_size = otm::PadToPowerOf2(sizeof T);
		return MemoryPool<block_size>::Get().Deallocate(ptr, count);
	}
	
	template <class T>
	struct Allocator
	{
		using value_type = T;

		constexpr Allocator() noexcept = default;

		template <class Y>
		constexpr Allocator(const Allocator<Y>&) noexcept {}
		
		T* allocate(size_t count) const
		{
			return Allocate<T>(count);
		}

		void deallocate(T* ptr, size_t count) const noexcept
		{
			Deallocate(ptr, count);
		}

		constexpr bool operator==(const Allocator&) const noexcept { return true; }
	};
}
