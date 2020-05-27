#pragma once
#include <atomic>
#include <compare>
#include <memory>

namespace oeng
{
	namespace detail
	{
		using RefCnt = std::conditional_t<OENG_SHARED_PTR_THREADSAFE, std::atomic_ulong, unsigned long>;

		struct SharedObjBase
		{
			bool IncStrongNz() noexcept
			{
#if OENG_SHARED_PTR_THREADSAFE
				auto count = strong.load();
				while (count != 0)
					if (strong.compare_exchange_strong(count, count+1, std::memory_order_relaxed))
						return true;
				return false;
#else
				if (strong == 0) return false;
				IncStrong();
				return true;
#endif
			}
			
			void IncStrong() noexcept
			{
#if OENG_SHARED_PTR_THREADSAFE
				strong.fetch_add(1, std::memory_order_relaxed);
#else
				++strong;
#endif
			}
			
			void IncWeak() noexcept
			{
#if OENG_SHARED_PTR_THREADSAFE
				weak.fetch_add(1, std::memory_order_relaxed);
#else
				++weak;
#endif
			}

			void DecStrong() noexcept
			{
				unsigned long old_strong;

#if OENG_SHARED_PTR_THREADSAFE
				old_strong = strong.fetch_sub(1, std::memory_order_acq_rel);
#else
				old_strong = strong--;
#endif

				if (old_strong == 1)
				{
					Destroy();
					DecWeak();
				}
			}

			void DecWeak() noexcept
			{
				unsigned long old_weak;
				
#if OENG_SHARED_PTR_THREADSAFE
				old_weak = weak.fetch_sub(1, std::memory_order_acq_rel);
#else
				old_weak = weak--;
#endif

				if (old_weak == 1) delete this;
			}

			unsigned long Strong() const noexcept
			{
#if OENG_SHARED_PTR_THREADSAFE
				return strong.load(std::memory_order_relaxed);
#else
				return strong;
#endif
			}
			
			unsigned long Weak() const noexcept
			{
#if OENG_SHARED_PTR_THREADSAFE
				return weak.load(std::memory_order_relaxed);
#else
				return weak;
#endif
			}

		protected:
			virtual ~SharedObjBase() = default;

		private:
			virtual void Destroy() noexcept = 0;

			RefCnt strong = 1;
			RefCnt weak = 1;
		};

		template <class T>
		struct SharedObjInline : SharedObjBase
		{
			template <class... Args>
			SharedObjInline(Args&&... args) :obj{ std::forward<Args>(args)... } {}

		private:
			~SharedObjInline() {}
			void Destroy() noexcept override { obj.~T(); }

			union { T obj; };
		};

		template <class T, class Deleter>
		struct SharedObjPtr : SharedObjBase
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
	}

	template <class T>
	class EnableSharedFromThis;

	template <class T>
	class WeakPtr;

	template <class T>
	class SharedPtr
	{
	public:
		constexpr SharedPtr() noexcept = default;
		constexpr SharedPtr(nullptr_t) noexcept {}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		explicit SharedPtr(Y* ptr) { Reset(ptr); }

		template <class Y, std::invocable<Y*> Deleter, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(Y* ptr, Deleter deleter) { Reset(ptr, std::move(deleter)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const SharedPtr<Y>& r, T* ptr) noexcept { AliasCopyFrom(r, ptr); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y>&& r, T* ptr) noexcept { AliasMoveFrom(std::move(r), ptr); }

		SharedPtr(const SharedPtr& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const SharedPtr<Y>& r) noexcept { CopyFrom(r); }

		SharedPtr(SharedPtr&& r) noexcept { MoveFrom(std::move(r)); }
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y>&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		explicit SharedPtr(const WeakPtr<Y>& r) { if (!FromWeak(r)) throw std::bad_weak_ptr{}; }

		~SharedPtr() { if (obj_) obj_->DecStrong(); }

		SharedPtr& operator=(const SharedPtr& r) noexcept
		{
			SharedPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr& operator=(const SharedPtr<Y>& r) noexcept
		{
			SharedPtr{r}.Swap(*this);
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& r) noexcept
		{
			SharedPtr{std::move(r)}.Swap(*this);
			return *this;
		}
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr& operator=(SharedPtr<Y>&& r) noexcept
		{
			SharedPtr{std::move(r)}.Swap(*this);
			return *this;
		}

		template <class Y = T, std::invocable<Y*> Deleter = std::default_delete<Y>, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void Reset(Y* ptr = nullptr, Deleter deleter = {})
		{
			if (obj_) obj_->DecStrong();
			ptr_ = ptr;
			obj_ = new detail::SharedObjPtr<Y, Deleter>{ptr, std::move(deleter)};
			if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, Y>)
			{
				ptr->weak_ = *this;
			}
		}

		void Swap(SharedPtr& r) noexcept
		{
			using std::swap;
			swap(ptr_, r.ptr_);
			swap(obj_, r.obj_);
		}

		[[nodiscard]] T* Get() const noexcept { return ptr_; }

		T& operator*() const noexcept { return *ptr_; }
		T* operator->() const noexcept { return ptr_; }

		[[nodiscard]] unsigned long UseCount() const noexcept { return obj_ ? obj_->Strong() : 0; }

		explicit operator bool() const noexcept { return ptr_; }
	
		template <class U>
		bool operator==(const SharedPtr<U>& r) const noexcept { return ptr_ == r.ptr_; }

		template <class U>
		std::strong_ordering operator<=>(const SharedPtr<U>& r) const noexcept
		{
			return ptr_ <=> r.ptr_;
		}

		bool operator==(nullptr_t) const noexcept { return ptr_ == nullptr; }
		std::strong_ordering operator<=>(nullptr_t) noexcept { return ptr_ <=> 0; }
		
	private:
		template <class Y>
		friend class WeakPtr;
		
		template <class Y>
		void CopyFrom(const SharedPtr<Y>& r) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = r.ptr_;
			obj_ = r.obj_;
		}
		
		template <class Y>
		void MoveFrom(SharedPtr<Y>&& r) noexcept
		{
			ptr_ = r.ptr_;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y>
		void AliasCopyFrom(const SharedPtr<Y>& r, T* ptr) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = ptr;
			obj_ = r.obj_;
		}
		
		template <class Y>
		void AliasMoveFrom(SharedPtr<Y>&& r, T* ptr) noexcept
		{
			ptr_ = ptr;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y>
		bool FromWeak(const WeakPtr<Y>& r) noexcept
		{
			if (r.obj_ && r.obj_->IncStrongNz())
			{
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				return true;
			}
			return false;
		}
		
		T* ptr_ = nullptr;
		detail::SharedObjBase* obj_ = nullptr;
	};

	template <class T>
	class WeakPtr
	{
	public:
		constexpr WeakPtr() noexcept = default;
		WeakPtr(const WeakPtr& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const WeakPtr<Y>& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const SharedPtr<Y>& r) noexcept { CopyFrom(r); }
		
		WeakPtr(WeakPtr&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(WeakPtr<Y>&& r) noexcept { MoveFrom(std::move(r)); }

		~WeakPtr() { if (obj_) obj_->DecWeak();	}
		
		WeakPtr& operator=(const WeakPtr& r) noexcept
		{
			WeakPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr& operator=(const WeakPtr<Y>& r) noexcept
		{
			WeakPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr& operator=(const SharedPtr<Y>& r) noexcept
		{
			WeakPtr{r}.Swap(*this);
			return *this;
		}
		
		WeakPtr& operator=(WeakPtr&& r) noexcept
		{
			WeakPtr{std::move(r)}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr& operator=(WeakPtr<Y>&& r) noexcept
		{
			WeakPtr{std::move(r)}.Swap(*this);
			return *this;
		}

		void Reset() noexcept
		{
			if (obj_) obj_->DecWeak();
			ptr_ = nullptr;
			obj_ = nullptr;
		}

		void Swap(WeakPtr& r) noexcept
		{
			using std::swap;
			swap(ptr_, r.ptr_);
			swap(obj_, r.obj_);
		}

		[[nodiscard]] unsigned long UseCount() const noexcept { return obj_ ? obj_->Strong() : 0; }
		[[nodiscard]] bool Expired() const noexcept { return UseCount() == 0; }
		
		[[nodiscard]] SharedPtr<T> Lock() const noexcept
		{
			SharedPtr<T> ret;
			ret.FromWeak(*this);
			return ret;
		}

	private:
		template <class Y>
		friend class SharedPtr;
		
		template <class Ptr>
		void CopyFrom(const Ptr& r) noexcept
		{
			if (r.obj_) r.obj_->IncWeak();
			ptr_ = r.ptr_;
			obj_ = r.obj_;
		}
		
		template <class Y>
		void MoveFrom(WeakPtr<Y>&& r) noexcept
		{
			ptr_ = r.ptr_;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}
		
		T* ptr_ = nullptr;
		detail::SharedObjBase* obj_ = nullptr;
	};

	template <class T>
	class EnableSharedFromThis
	{
	public:
		[[nodiscard]] WeakPtr<T> WeakFromThis() noexcept { return weak_; }
		[[nodiscard]] WeakPtr<const T> WeakFromThis() const noexcept { return weak_; }
		[[nodiscard]] SharedPtr<T> SharedFromThis() noexcept { return SharedPtr<T>{weak_}; }
		[[nodiscard]] SharedPtr<const T> SharedFromThis() const noexcept { return SharedPtr<const T>{weak_}; }

	protected:
		constexpr EnableSharedFromThis() noexcept = default;
		EnableSharedFromThis(const EnableSharedFromThis&) noexcept {}
		EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept { return *this; }
		
	private:
		friend class SharedPtr<T>;
		WeakPtr<T> weak_;
	};

	template <class T>
	void swap(SharedPtr<T>& l, SharedPtr<T>& r) noexcept { l.Swap(r); }

	template <class T>
	void swap(WeakPtr<T>& l, WeakPtr<T>& r) noexcept { l.Swap(r); }

	template <class T>
	SharedPtr(WeakPtr<T>) -> SharedPtr<T>;

	template <class T>
	WeakPtr(SharedPtr<T>) -> WeakPtr<T>;
}

template <class T>
struct std::hash<oeng::SharedPtr<T>>
{
	size_t operator()(const oeng::SharedPtr<T>& p) const noexcept
	{
		return std::hash<T*>{}(p.Get());
	}
};
