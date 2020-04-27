#pragma once
#include <atomic>
#include <memory>
#include "CompressedPair.hpp"

namespace oeng
{
	namespace detail
	{
		template <class T, bool ThreadSafe>
		struct ManagedObjBase
		{
			virtual ~ManagedObjBase() = default;
			ManagedObjBase(const ManagedObjBase&) = delete;
			ManagedObjBase(ManagedObjBase&&) = delete;
			ManagedObjBase& operator=(const ManagedObjBase&) = delete;
			ManagedObjBase& operator=(ManagedObjBase&&) = delete;

		private:
			virtual void Destroy() noexcept = 0;
			virtual void DeleteSelf() noexcept = 0;

			bool valid = true;
			using CntT = uint8_t;
			std::conditional_t<ThreadSafe, std::atomic<CntT>, CntT> ref_cnt = 1;
		};

		template <class T, bool ThreadSafe>
		struct ManagedObjInline final : ManagedObjBase<T, ThreadSafe>
		{
			template <class... Args>
			constexpr explicit ManagedObjInline(Args&&... args):
				ManagedObjBase<T, ThreadSafe>{}, obj{std::forward<Args>(args)...}
			{
			}

			~ManagedObjInline() { /* Do nothing. obj is already destroyed. */ }
			
			ManagedObjInline(const ManagedObjInline&) = delete;
			ManagedObjInline(ManagedObjInline&&) = delete;
			ManagedObjInline& operator=(const ManagedObjInline&) = delete;
			ManagedObjInline& operator=(ManagedObjInline&&) = delete;
			
		private:
			void Destroy() noexcept override { obj.~T(); }
			void DeleteSelf() noexcept override { delete this; }
			
			union { T obj; };
		};

		template <class T, class Al, bool ThreadSafe>
		struct ManagedObjInAl final : ManagedObjBase<T, ThreadSafe>
		{
			template <class... Args>
			constexpr explicit ManagedObjInAl(Al ax, Args&&... args):
				ManagedObjBase<T, ThreadSafe>{},
				pair{OneThen{}, std::move(ax), std::forward<Args>(args)...}
			{
			}
			
			~ManagedObjInAl() { pair.first.~T(); }

			ManagedObjInAl(const ManagedObjInAl&) = delete;
			ManagedObjInAl(ManagedObjInAl&&) = delete;
			ManagedObjInAl& operator=(const ManagedObjInAl&) = delete;
			ManagedObjInAl& operator=(ManagedObjInAl&&) = delete;
			
		private:
			void Destroy() noexcept override
			{
				std::allocator_traits<Al>::destroy(pair.first, &pair.second);
			}
			
			void DeleteSelf() noexcept override
			{
				using Alloc = typename std::allocator_traits<Al>::template rebind_alloc<ManagedObjInAl>;
				Alloc ax = std::move(pair.first);
				std::allocator_traits<Alloc>::destroy(ax, this);
				std::allocator_traits<Alloc>::deallocate(ax, this, 1);
			}
			
			union { CompressedPair<Al, T> pair; };
		};

		template <class T, class Al, class Dx, bool ThreadSafe>
		struct ManagedObjPtr final : ManagedObjBase<T, ThreadSafe>
		{
			ManagedObjPtr(T* ptr, Dx dt, Al ax):
				ManagedObjBase<T, ThreadSafe>{},
				pair{OneThen{}, std::move(ax), OneThen{}, std::move(dt), ptr}
			{
			}
			
		private:
			using Alloc = typename std::allocator_traits<Al>::template rebind_alloc<ManagedObjPtr>;
			
			void Destroy() noexcept override { pair.second.first(pair.second.second); }
			void DeleteSelf() noexcept override
			{
				std::allocator_traits<Alloc>::destroy(pair.first, this);
				std::allocator_traits<Alloc>::deallocate(pair.first, this, 1);
			}

			CompressedPair<Alloc, CompressedPair<Dx, T*>> pair;
		};

		template <class T, bool ThreadSafe>
		class ManagedPtr
		{
		protected:
			explicit constexpr ManagedPtr(ManagedObjBase<T, ThreadSafe>* ptr = nullptr) noexcept: ptr_{ptr} {}
			ManagedObjBase<T, ThreadSafe>* ptr_;
		};
	}

	template <class T, bool ThreadSafe = false>
	class OwnerPtr : detail::ManagedPtr<T, ThreadSafe>
	{
	public:
		OwnerPtr();
	};
}
