#pragma once
#include <mutex>

namespace oeng
{
	template <class T>
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
