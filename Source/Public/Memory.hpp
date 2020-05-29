#pragma once
#include <vector>
#include <fmt/core.h>
#include "Assert.hpp"
#include "otm/Basic.hpp"

namespace oeng
{
	template <size_t BlockSizeLog2>
	class MemoryPool
	{
	public:
		static constexpr auto BlockSize = 1 << BlockSizeLog2;
		
		static MemoryPool& Get() noexcept
		{
			thread_local MemoryPool pool;
			return pool;
		}

		template <class T>
		T* Allocate(const size_t count)
		{
			const auto need_bytes = count * sizeof T;
			const auto need_blocks = CountBlocks(need_bytes);
			
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
			peak_blocks_ = otm::Max(peak_blocks_, total_blocks_);
			return Allocate<T>(containers_.emplace_back(new_blocks), 0, need_blocks);
		}
		
		template <class T>
		void Deallocate(T* ptr, size_t count) noexcept
		{
			const auto p = reinterpret_cast<Block*>(ptr);
			
			auto it = containers_.begin();
			IF_NOT_ENSURE_MSG(it != containers_.end(), "Attempted to deallocate while no memory allocated") return;
			
			while (it->blocks.data() > p || p >= it->blocks.data() + it->blocks.size())
			{
				++it;
				IF_NOT_ENSURE_MSG(it != containers_.end(), "Attempted to deallocate invalid pointer") return;
			}
			
			const auto begin = p - it->blocks.data();
			const auto blocks = CountBlocks(sizeof T * count);
			for (auto i = begin; i < begin+blocks; ++i)
			{
				ENSURE_LOG_MSG(it->occupied[i], "Deallocating empty block");
				it->occupied[i] = false;
			}
			
			it->first_available = otm::Min(it->first_available, begin);
			
			IF_ENSURE_MSG(it->remaining >= blocks, "Deallocating more than remaining")
			{
				it->remaining -= blocks;
			}
			else it->remaining = 0;
			
			if (it->remaining == 0) containers_.erase(it);
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

		static constexpr size_t CountBlocks(size_t bytes) noexcept
		{
			return bytes/BlockSize + otm::Min(1, bytes%BlockSize);
		}

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
		~MemoryPool()
		{
			ENSURE_LOG_MSG(containers_.empty(), "Memory leak detected");
			
			fmt::print("Peak usage of {} byte memory pool: {} blocks ({} bytes)",
				BlockSize, peak_blocks_, BlockSize*peak_blocks_);
		}

		std::vector<BlockContainer> containers_;
		size_t total_blocks_ = 0;
		size_t peak_blocks_ = 0;
	};

	template <class T>
	T* Allocate(size_t count)
	{
		return MemoryPool<otm::Log2Ceil(sizeof T)>::Get().template Allocate<T>(count);
	}
	
	template <class T>
	void Deallocate(T* ptr, size_t count) noexcept
	{
		return MemoryPool<otm::Log2Ceil(sizeof T)>::Get().Deallocate(ptr, count);
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
