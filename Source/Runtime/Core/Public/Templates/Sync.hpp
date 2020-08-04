#pragma once
#include <atomic>
#include <mutex>

CORE_BEGIN

class NullMutex
{
public:
	constexpr void lock() const noexcept {}
	constexpr bool try_lock() const noexcept { return true; }
	constexpr void unlock() const noexcept {}
};

template <class T>
struct NullAtomic
{
	constexpr NullAtomic() noexcept = default;
	constexpr NullAtomic(T desired) noexcept :data{std::move(desired)} {}
	NullAtomic(const NullAtomic&) = delete;
	NullAtomic(NullAtomic&&) = delete;
	~NullAtomic() = default;

	constexpr NullAtomic& operator=(T desired) noexcept { data = std::move(desired); return *this; }
	NullAtomic& operator=(const NullAtomic&) = delete;
	NullAtomic& operator=(NullAtomic&&) = delete;

	constexpr void store(T desired, std::memory_order = {}) noexcept { data = std::move(desired); }
	constexpr T load(std::memory_order = {}) const noexcept { return data; }
	constexpr operator T() const noexcept { return data; }

	constexpr T exchange(T desired, std::memory_order = {}) noexcept
	{
		using std::swap;
		swap(data, desired);
		return desired;
	}

	constexpr bool compare_exchange_weak(T& expected, T desired, std::memory_order = {}, std::memory_order = {}) noexcept
	{
		if (data == expected)
		{
			data = std::move(desired);
			return true;
		}
		expected = data;
		return false;
	}

	constexpr bool compare_exchange_strong(T& expected, T desired, std::memory_order = {}, std::memory_order = {}) noexcept
	{
		return compare_exchange_weak(expected, desired);
	}

	constexpr T fetch_add(T arg, std::memory_order = {}) noexcept { auto old = data; data += std::move(arg); return old; }
	constexpr T fetch_sub(T arg, std::memory_order = {}) noexcept { auto old = data; data -= std::move(arg); return old; }
	constexpr T fetch_and(T arg, std::memory_order = {}) noexcept { auto old = data; data &= std::move(arg); return old; }
	constexpr T fetch_or(T arg, std::memory_order = {}) noexcept { auto old = data; data |= std::move(arg); return old; }
	constexpr T fetch_xor(T arg, std::memory_order = {}) noexcept { auto old = data; data ^= std::move(arg); return old; }

	constexpr T operator++() noexcept { return ++data; }
	constexpr T operator++(int) noexcept { return data++; }
	constexpr T operator--() noexcept { return --data; }
	constexpr T operator--(int) noexcept { return data--; }

	constexpr T operator+=(T arg) noexcept { return data += std::move(arg); }
	constexpr T operator-=(T arg) noexcept { return data -= std::move(arg); }
	constexpr T operator&=(T arg) noexcept { return data &= std::move(arg); }
	constexpr T operator|=(T arg) noexcept { return data |= std::move(arg); }
	constexpr T operator^=(T arg) noexcept { return data ^= std::move(arg); }

private:
	T data;
};

template <bool ThreadSafe>
using CondMutex = std::conditional_t<ThreadSafe, std::mutex, NullMutex>;

template <class T, bool ThreadSafe>
using CondAtomic = std::conditional_t<ThreadSafe, std::atomic<T>, NullAtomic<T>>;

template <class T, class Mutex = std::mutex>
class Monitor
{
	struct Handle
	{
		explicit Handle(Monitor& set) :object{set.object_}, lock{set.mutex_} {}
		T* operator->() const noexcept { return &object; }
		T& operator*() const noexcept { return object; }
		
	private:
		T& object;
		std::lock_guard<Mutex> lock;
	};
	
	struct ConstHandle
	{
		explicit ConstHandle(const Monitor& set) :object{set.object_}, lock{set.mutex_} {}
		const T* operator->() const noexcept { return &object; }
		const T& operator*() const noexcept { return object; }
		
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
	Handle operator*() { return Lock(); }
	ConstHandle operator->() const { return Lock(); }
	ConstHandle operator*() const { return Lock(); }

	[[nodiscard]] Handle Lock() { return Handle{*this}; }
	[[nodiscard]] ConstHandle Lock() const { return ConstHandle{*this}; }

	[[nodiscard]] T& GetUnsafe() noexcept { return object_; }
	[[nodiscard]] const T& GetUnsafe() const noexcept { return object_; }
};

template <class T, bool ThreadSafe>
using CondMonitor = Monitor<T, CondMutex<ThreadSafe>>;

CORE_END
