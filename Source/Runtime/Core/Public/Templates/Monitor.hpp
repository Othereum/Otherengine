#pragma once
#include <mutex>

namespace oeng
{
	class NullMutex
	{
	public:
		constexpr void lock() const noexcept {}
		constexpr bool try_lock() const noexcept { return true; }
		constexpr void unlock() const noexcept {}
	};

	template <bool ThreadSafe>
	using CondMutex = std::conditional_t<ThreadSafe, std::mutex, NullMutex>;
	
	template <class T, class Mutex = std::mutex>
	class Monitor
	{
		struct Handle
		{
			explicit Handle(Monitor& set) :object{set.object_}, lock{set.mutex_} {}
			T* operator->() const noexcept { return &object; }
			
		private:
			T& object;
			std::lock_guard<Mutex> lock;
		};
		
		struct ConstHandle
		{
			explicit ConstHandle(const Monitor& set) :object{set.object_}, lock{set.mutex_} {}
			const T* operator->() const noexcept { return &object; }
			
		private:
			const T& object;
			std::lock_guard<Mutex> lock;
		};
		
		T object_;
		[[no_unique_address]] Mutex mutex_;
		
	public:
		template <class... Args>
		explicit(sizeof...(Args) == 1)
		Monitor(Args&&... args) :object_{std::forward<Args>(args)...} {}

		Handle operator->() { return Lock(); }
		ConstHandle operator->() const noexcept { return Lock(); }

		[[nodiscard]] Handle Lock() { return Handle{*this}; }
		[[nodiscard]] ConstHandle Lock() const { return ConstHandle{*this}; }

		[[nodiscard]] T& GetUnsafe() noexcept { return object_; }
		[[nodiscard]] const T& GetUnsafe() const noexcept { return object_; }
	};
}
