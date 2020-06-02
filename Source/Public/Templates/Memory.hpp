#pragma once
#include <atomic>
#include <memory>
#include "Assert.hpp"

namespace oeng
{
	[[nodiscard]] inline void* Alloc(size_t n)
	{
		return operator new(n);
	}

	inline void Free(void* p) noexcept
	{
		operator delete(p);
	}
	
	template <class T, class... Args>
	[[nodiscard]] T* New(Args&&... args)
	{
		return new T{std::forward<Args>(args)...};
	}

	template <class T, class... Args>
	[[nodiscard]] T* NewArr(size_t n, Args&&... args)
	{
		return new T[n]{std::forward<Args>(args)...};
	}

	template <class T>
	void Delete(T* p) noexcept
	{
		delete p;
	}

	template <class T>
	void DeleteArr(T* p) noexcept
	{
		delete[] p;
	}

	template <class T>
	using Allocator = std::allocator<T>;

	template <class T>
	using DefaultDelete = std::default_delete<T>;

	template <class T, class Deleter = DefaultDelete<T>>
	using UniquePtr = std::unique_ptr<T, Deleter>;

	template <class T, class... Args>
	UniquePtr<T> MakeUnique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	namespace detail
	{
		template <bool ThreadSafe>
		struct SharedObjBase  // NOLINT(cppcoreguidelines-special-member-functions)
		{
			virtual ~SharedObjBase() = default;

			bool IncStrongNz() noexcept
			{
				if constexpr (ThreadSafe)
				{
					auto count = strong.load();
					while (count != 0)
						if (strong.compare_exchange_strong(count, count+1, std::memory_order_relaxed))
							return true;
					return false;
				}
				else
				{
					if (strong == 0) return false;
					IncStrong();
					return true;
				}
			}

			void IncStrong() noexcept
			{
				if constexpr (ThreadSafe)
				{
					strong.fetch_add(1, std::memory_order_relaxed);
				}
				else
				{
					++strong;
				}
			}

			void IncWeak() noexcept
			{
				if constexpr (ThreadSafe)
				{
					weak.fetch_add(1, std::memory_order_relaxed);
				}
				else
				{
					++weak;
				}
			}

			void DecStrong() noexcept
			{
				unsigned long old_strong;

				if constexpr (ThreadSafe)
				{
					old_strong = strong.fetch_sub(1, std::memory_order_acq_rel);
				}
				else
				{
					old_strong = strong--;
				}

				if (old_strong == 1)
				{
					Destroy();
					DecWeak();
				}
			}

			void DecWeak() noexcept
			{
				unsigned long old_weak;

				if constexpr (ThreadSafe)
				{
					old_weak = weak.fetch_sub(1, std::memory_order_acq_rel);
				}
				else
				{
					old_weak = weak--;
				}

				if (old_weak == 1) delete this;
			}

			[[nodiscard]] unsigned long Strong() const noexcept
			{
				if constexpr (ThreadSafe)
				{
					return strong.load(std::memory_order_relaxed);
				}
				else
				{
					return strong;
				}
			}

			[[nodiscard]] unsigned long Weak() const noexcept
			{
				if constexpr (ThreadSafe)
				{
					return weak.load(std::memory_order_relaxed);
				}
				else
				{
					return weak;
				}
			}

		private:
			virtual void Destroy() noexcept = 0;

			using RefCnt = std::conditional_t<ThreadSafe, std::atomic_ulong, unsigned long>;
			RefCnt strong = 1;
			RefCnt weak = 1;
		};

		template <class T, bool ThreadSafe>
		struct SharedObjInline : SharedObjBase<ThreadSafe>  // NOLINT(cppcoreguidelines-special-member-functions)
		{
			template <class... Args>
			SharedObjInline(Args&&... args)
				:obj{ std::forward<Args>(args)... }
			{
			}

			~SharedObjInline() {}

			union { T obj; };

		private:
			void Destroy() noexcept override { obj.~T(); }
		};

		template <class T, class Deleter, bool ThreadSafe>
		struct SharedObjPtr : SharedObjBase<ThreadSafe>
		{
			SharedObjPtr(T* ptr, Deleter deleter) noexcept
				:ptr{ptr}, deleter{std::move(deleter)}
			{
			}

		private:
			void Destroy() noexcept override { if (ptr) deleter(ptr); }

			T* ptr;
			[[no_unique_address]] Deleter deleter;
		};

		template <class T, class = void>
		struct CanEnableShared : std::false_type {};

		template <class T>
		struct CanEnableShared<T, std::void_t<typename T::EsftType>>
			: std::is_convertible<std::remove_cv_t<T>*, typename T::EsftType*>::type
		{
		};
	}

	template <class T, bool ThreadSafe = OE_SHARED_PTR_THREADSAFE>
	class EnableSharedFromThis;

	template <class T, bool ThreadSafe = OE_SHARED_PTR_THREADSAFE>
	class WeakPtr;

	template <class T, bool ThreadSafe = OE_SHARED_PTR_THREADSAFE>
	class SharedPtr
	{
	public:
		constexpr SharedPtr() noexcept = default;
		constexpr SharedPtr(nullptr_t) noexcept {}

		template <class Y>
		explicit SharedPtr(Y* ptr) { reset(ptr); }

		template <class Y, class Deleter>
		SharedPtr(Y* ptr, Deleter deleter) { reset(ptr, std::move(deleter)); }

		template <class Y>
		SharedPtr(const SharedPtr<Y, ThreadSafe>& r, T* ptr) noexcept { AliasCopyFrom(r, ptr); }

		template <class Y>
		SharedPtr(SharedPtr<Y, ThreadSafe>&& r, T* ptr) noexcept { AliasMoveFrom(std::move(r), ptr); }

		SharedPtr(const SharedPtr& r) noexcept { CopyFrom(r); }

		template <class Y>
		SharedPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }

		SharedPtr(SharedPtr&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y>
		SharedPtr(SharedPtr<Y, ThreadSafe>&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y>
		explicit SharedPtr(const WeakPtr<Y, ThreadSafe>& r) { if (!FromWeak(r)) throw std::bad_weak_ptr{}; }

		~SharedPtr() { reset(); }

		SharedPtr& operator=(const SharedPtr& r) noexcept  // NOLINT(bugprone-unhandled-self-assignment)
		{
			SharedPtr{r}.swap(*this);
			return *this;
		}

		template <class Y>
		SharedPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
		{
			SharedPtr{r}.swap(*this);
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& r) noexcept
		{
			SharedPtr{std::move(r)}.swap(*this);
			return *this;
		}

		template <class Y>
		SharedPtr& operator=(SharedPtr<Y, ThreadSafe>&& r) noexcept
		{
			SharedPtr{std::move(r)}.swap(*this);
			return *this;
		}

		void reset() noexcept
		{
			if (obj_) obj_->DecStrong();
			obj_ = nullptr;
			ptr_ = nullptr;
		}

		void reset(nullptr_t) noexcept { reset(); }

		template <class Y = T, class Deleter = DefaultDelete<Y>>
		void reset(Y* ptr, Deleter deleter = {})
		{
			if (obj_) obj_->DecStrong();
			SetAndEnableShared(ptr, New<detail::SharedObjPtr<Y, Deleter, ThreadSafe>>(ptr, std::move(deleter)));
		}

		void swap(SharedPtr& r) noexcept
		{
			using std::swap;
			swap(ptr_, r.ptr_);
			swap(obj_, r.obj_);
		}

		[[nodiscard]] T* get() const noexcept { return ptr_; }

		T& operator*() const noexcept { CHECK_SLOW(ptr_); return *ptr_; }
		T* operator->() const noexcept { CHECK_SLOW(ptr_); return ptr_; }

		[[nodiscard]] unsigned long use_count() const noexcept { return obj_ ? obj_->Strong() : 0; }

		explicit operator bool() const noexcept { return ptr_; }

		template <class U>
		constexpr bool operator==(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ == r.ptr_; }

		template <class U>
		constexpr bool operator!=(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ != r.ptr_; }

		template <class U>
		constexpr bool operator<(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ < r.ptr_; }

		template <class U>
		constexpr bool operator>(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ > r.ptr_; }

		template <class U>
		constexpr bool operator<=(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ <= r.ptr_; }

		template <class U>
		constexpr bool operator>=(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ >= r.ptr_; }

		constexpr bool operator==(nullptr_t) const noexcept { return ptr_ == nullptr; }
		constexpr bool operator!=(nullptr_t) const noexcept { return ptr_ != nullptr; }
		constexpr bool operator<(nullptr_t) const noexcept { return ptr_ < nullptr; }
		constexpr bool operator>(nullptr_t) const noexcept { return ptr_ > nullptr; }
		constexpr bool operator<=(nullptr_t) const noexcept { return ptr_ <= nullptr; }
		constexpr bool operator>=(nullptr_t) const noexcept { return ptr_ >= nullptr; }

	private:
		template <class, bool>
		friend class SharedPtr;

		template <class, bool>
		friend class WeakPtr;

		template <class Y, bool TSafe, class... Args>
		friend SharedPtr<Y, TSafe> MakeShared(Args&&... args);

		template <class Y>
		void CopyFrom(const SharedPtr<Y, ThreadSafe>& r) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = r.ptr_;
			obj_ = r.obj_;
		}

		template <class Y>
		void MoveFrom(SharedPtr<Y, ThreadSafe>&& r) noexcept
		{
			ptr_ = r.ptr_;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y>
		void AliasCopyFrom(const SharedPtr<Y, ThreadSafe>& r, T* ptr) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = ptr;
			obj_ = r.obj_;
		}

		template <class Y>
		void AliasMoveFrom(SharedPtr<Y, ThreadSafe>&& r, T* ptr) noexcept
		{
			ptr_ = ptr;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y>
		bool FromWeak(const WeakPtr<Y, ThreadSafe>& r) noexcept
		{
			if (r.obj_ && r.obj_->IncStrongNz())
			{
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				return true;
			}
			return false;
		}

		template <class Y>
		void SetAndEnableShared(Y* ptr, detail::SharedObjBase<ThreadSafe>* obj) noexcept
		{
			ptr_ = ptr;
			obj_ = obj;
			if constexpr (detail::CanEnableShared<Y>::value)
			{
				if (ptr_)
				{
					CHECK_SLOW(ptr_->weak_.Expired());
					ptr->weak_ = SharedPtr<std::remove_cv_t<Y>, ThreadSafe>{*this, const_cast<std::remove_cv_t<Y>*>(ptr)};
				}
			}
		}

		T* ptr_ = nullptr;
		detail::SharedObjBase<ThreadSafe>* obj_ = nullptr;
	};

	template <class T, bool ThreadSafe>
	class WeakPtr
	{
	public:
		constexpr WeakPtr() noexcept = default;
		WeakPtr(const WeakPtr& r) noexcept { CopyFrom(r); }

		template <class Y>
		WeakPtr(const WeakPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }

		template <class Y>
		WeakPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }

		WeakPtr(WeakPtr&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y>
		WeakPtr(WeakPtr<Y, ThreadSafe>&& r) noexcept { MoveFrom(std::move(r)); }

		~WeakPtr() { if (obj_) obj_->DecWeak();	}

		WeakPtr& operator=(const WeakPtr& r) noexcept  // NOLINT(bugprone-unhandled-self-assignment)
		{
			WeakPtr{r}.swap(*this);
			return *this;
		}

		template <class Y>
		WeakPtr& operator=(const WeakPtr<Y, ThreadSafe>& r) noexcept
		{
			WeakPtr{r}.swap(*this);
			return *this;
		}

		template <class Y>
		WeakPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
		{
			WeakPtr{r}.swap(*this);
			return *this;
		}

		WeakPtr& operator=(WeakPtr&& r) noexcept
		{
			WeakPtr{std::move(r)}.swap(*this);
			return *this;
		}

		template <class Y>
		WeakPtr& operator=(WeakPtr<Y, ThreadSafe>&& r) noexcept
		{
			WeakPtr{std::move(r)}.swap(*this);
			return *this;
		}

		void reset() noexcept
		{
			if (obj_) obj_->DecWeak();
			ptr_ = nullptr;
			obj_ = nullptr;
		}

		void swap(WeakPtr& r) noexcept
		{
			using std::swap;
			swap(ptr_, r.ptr_);
			swap(obj_, r.obj_);
		}

		[[nodiscard]] unsigned long use_count() const noexcept { return obj_ ? obj_->Strong() : 0; }
		[[nodiscard]] bool expired() const noexcept { return use_count() == 0; }

		[[nodiscard]] SharedPtr<T, ThreadSafe> lock() const noexcept
		{
			SharedPtr<T, ThreadSafe> ret;
			ret.FromWeak(*this);
			return ret;
		}

	private:
		template <class, bool>
		friend class SharedPtr;

		template <class, bool>
		friend class WeakPtr;

		template <class Ptr>
		void CopyFrom(const Ptr& r) noexcept
		{
			if (r.obj_) r.obj_->IncWeak();
			ptr_ = r.ptr_;
			obj_ = r.obj_;
		}

		template <class Y>
		void MoveFrom(WeakPtr<Y, ThreadSafe>&& r) noexcept
		{
			ptr_ = r.ptr_;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		T* ptr_ = nullptr;
		detail::SharedObjBase<ThreadSafe>* obj_ = nullptr;
	};

	template <class T, bool ThreadSafe>
	class EnableSharedFromThis  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		using EsftType = EnableSharedFromThis;

		[[nodiscard]] WeakPtr<T, ThreadSafe> weak_from_this() noexcept { return weak_; }
		[[nodiscard]] WeakPtr<const T, ThreadSafe> weak_from_this() const noexcept { return weak_; }
		[[nodiscard]] SharedPtr<T, ThreadSafe> shared_from_this() noexcept { return SharedPtr<T, ThreadSafe>{weak_}; }
		[[nodiscard]] SharedPtr<const T, ThreadSafe> shared_from_this() const noexcept { return SharedPtr<const T, ThreadSafe>{weak_}; }

	protected:
		constexpr EnableSharedFromThis() noexcept :weak_{} {}
		EnableSharedFromThis(const EnableSharedFromThis&) noexcept :weak_{} {}
		EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept { return *this; }
		~EnableSharedFromThis() = default;

	private:
		friend SharedPtr<T, ThreadSafe>;
		friend detail::SharedObjInline<T, ThreadSafe>;
		mutable WeakPtr<T, ThreadSafe> weak_;
	};

	template <class T, bool ThreadSafe = OE_SHARED_PTR_THREADSAFE, class... Args>
	SharedPtr<T, ThreadSafe> MakeShared(Args&&... args)
	{
		SharedPtr<T, ThreadSafe> ret;
		auto obj = New<detail::SharedObjInline<T, ThreadSafe>>(std::forward<Args>(args)...);
		ret.SetAndEnableShared(&obj->obj, obj);
		return ret;
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> StaticCast(const SharedPtr<U, ThreadSafe>& r) noexcept
	{
		auto p = static_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{r, p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> StaticCast(SharedPtr<U, ThreadSafe>&& r) noexcept
	{
		auto p = static_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{std::move(r), p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> ConstCast(const SharedPtr<U, ThreadSafe>& r) noexcept
	{
		auto p = const_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{r, p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> ConstCast(SharedPtr<U, ThreadSafe>&& r) noexcept
	{
		auto p = const_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{std::move(r), p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> DynamicCast(const SharedPtr<U, ThreadSafe>& r) noexcept
	{
		auto p = dynamic_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{r, p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> DynamicCast(SharedPtr<U, ThreadSafe>&& r) noexcept
	{
		auto p = dynamic_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{std::move(r), p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> ReinterpretCast(const SharedPtr<U, ThreadSafe>& r) noexcept
	{
		auto p = reinterpret_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{r, p};
	}

	template <class T, class U, bool ThreadSafe>
	SharedPtr<T, ThreadSafe> ReinterpretCast(SharedPtr<U, ThreadSafe>&& r) noexcept
	{
		auto p = reinterpret_cast<T*>(r.get());
		return SharedPtr<T, ThreadSafe>{std::move(r), p};
	}

	template <class T, bool ThreadSafe>
	constexpr bool operator==(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p == nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator!=(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p != nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator<(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p < nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator>(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p > nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator<=(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p <= nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator>=(nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p >= nullptr; }

	template <class T, bool ThreadSafe>
	void swap(SharedPtr<T, ThreadSafe>& l, SharedPtr<T, ThreadSafe>& r) noexcept { l.Swap(r); }

	template <class T, bool ThreadSafe>
	void swap(WeakPtr<T, ThreadSafe>& l, WeakPtr<T, ThreadSafe>& r) noexcept { l.Swap(r); }

	template <class T, bool ThreadSafe>
	SharedPtr(WeakPtr<T, ThreadSafe>) -> SharedPtr<T, ThreadSafe>;

	template <class T, bool ThreadSafe>
	WeakPtr(SharedPtr<T, ThreadSafe>) -> WeakPtr<T, ThreadSafe>;
}

template <class T, bool ThreadSafe>
struct std::hash<oeng::SharedPtr<T, ThreadSafe>>
{
	size_t operator()(const oeng::SharedPtr<T, ThreadSafe>& p) const noexcept
	{
		return std::hash<T*>{}(p.Get());
	}
};
