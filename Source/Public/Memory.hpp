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

		template <class T>
		T* Allocate(const size_t count)
		{
			const auto need_bytes = count * sizeof T;
			const auto need_blocks = need_bytes/BlockSize + otm::Min(1, need_bytes%BlockSize);
			
			for (auto& c : containers_)
			{
				if (c.remaining * BlockSize < need_bytes) continue;
				
				size_t continuous = 0;
				for (size_t cur_block = c.first_available; cur_block < c.blocks.size(); ++cur_block)
				{
					if (c.occupied[cur_block]) continuous = 0;
					else if (++continuous * BlockSize >= need_bytes)
					{
						const auto begin = cur_block - need_blocks + 1;
						return Allocate<T>(c, begin, need_blocks);
					}
				}
			}

			const auto new_blocks = otm::Max(total_blocks_ / 2, need_blocks);
			total_blocks_ += new_blocks;
			return Allocate<T>(containers_.emplace_back(new_blocks), 0, need_blocks);
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
			// ReSharper disable once CppPossiblyUninitializedMember
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

		template <class T>
		static T* Allocate(BlockContainer& c, size_t begin, size_t blocks)
		{
			const auto end = begin + blocks;
			auto i = begin;
			for (; i < end; ++i) c.occupied[i] = true;
			for (; i < c.blocks.size() && c.occupied[i]; ++i) {}
			c.first_available = i;
			c.remaining -= blocks;
			return reinterpret_cast<T*>(c.blocks.data() + begin);
		}

		MemoryPool() = default;
		
		std::vector<BlockContainer> containers_;
		size_t total_blocks_ = 0;
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
