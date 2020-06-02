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
			std::lock_guard<std::mutex> lock;
		};
		
		T object_;
		std::mutex mutex_;
		
	public:
		template <class... Args>
		explicit(sizeof...(Args) == 1)
		Monitor(Args&&... args) :object_{std::forward<Args>(args)...} {}

		Handle operator->() { return Lock(); }
		const T* operator->() const noexcept { return &object_; }

		[[nodiscard]] Handle Lock() { return Handle{*this}; }
		[[nodiscard]] const T& Get() const noexcept { return object_; }
		[[nodiscard]] T& GetUnsafe() noexcept { return object_; }
	};
}
