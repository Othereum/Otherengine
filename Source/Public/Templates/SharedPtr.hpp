#pragma once
#include <cassert>
#include <atomic>
#include <compare>
#include <memory>

namespace oeng
{
	namespace detail
	{
		template <bool ThreadSafe>
		struct SharedObjBase
		{
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

			unsigned long Strong() const noexcept
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
			
			unsigned long Weak() const noexcept
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

		protected:
			virtual ~SharedObjBase() = default;

		private:
			virtual void Destroy() noexcept = 0;

			using RefCnt = std::conditional_t<ThreadSafe, std::atomic_ulong, unsigned long>;
			RefCnt strong = 1;
			RefCnt weak = 1;
		};

		template <class T, bool ThreadSafe>
		struct SharedObjInline : SharedObjBase<ThreadSafe>
		{
			template <class... Args>
			SharedObjInline(Args&&... args) :obj{ std::forward<Args>(args)... } {}

			union { T obj; };
			
		private:
			~SharedObjInline() {}
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

	template <class T, bool ThreadSafe = OENG_SHARED_PTR_THREADSAFE>
	class EnableSharedFromThis;

	template <class T, bool ThreadSafe = OENG_SHARED_PTR_THREADSAFE>
	class WeakPtr;

	template <class T, bool ThreadSafe = OENG_SHARED_PTR_THREADSAFE>
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
		SharedPtr(const SharedPtr<Y, ThreadSafe>& r, T* ptr) noexcept { AliasCopyFrom(r, ptr); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y, ThreadSafe>&& r, T* ptr) noexcept { AliasMoveFrom(std::move(r), ptr); }

		SharedPtr(const SharedPtr& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }

		SharedPtr(SharedPtr&& r) noexcept { MoveFrom(std::move(r)); }
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y, ThreadSafe>&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		explicit SharedPtr(const WeakPtr<Y, ThreadSafe>& r) { if (!FromWeak(r)) throw std::bad_weak_ptr{}; }

		~SharedPtr() { if (obj_) obj_->DecStrong(); }

		SharedPtr& operator=(const SharedPtr& r) noexcept
		{
			SharedPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
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
		SharedPtr& operator=(SharedPtr<Y, ThreadSafe>&& r) noexcept
		{
			SharedPtr{std::move(r)}.Swap(*this);
			return *this;
		}

		template <class Y = T, std::invocable<Y*> Deleter = std::default_delete<Y>, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void Reset(Y* ptr = nullptr, Deleter deleter = {})
		{
			if (obj_) obj_->DecStrong();
			SetAndEnableShared(ptr, new detail::SharedObjPtr<Y, Deleter, ThreadSafe>{ptr, std::move(deleter)});
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
		bool operator==(const SharedPtr<U, ThreadSafe>& r) const noexcept { return ptr_ == r.ptr_; }

		template <class U>
		std::strong_ordering operator<=>(const SharedPtr<U, ThreadSafe>& r) const noexcept
		{
			return ptr_ <=> r.ptr_;
		}

		bool operator==(nullptr_t) const noexcept { return ptr_ == nullptr; }
		std::strong_ordering operator<=>(nullptr_t) noexcept { return ptr_ <=> 0; }
		
	private:
		template <class Y>
		friend class WeakPtr<Y, ThreadSafe>;

		template <class Y, class... Args>
		friend SharedPtr<Y, ThreadSafe> MakeShared(Args&&... args);
		
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
					assert(ptr_->weak_.Expired());
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

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const WeakPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept { CopyFrom(r); }
		
		WeakPtr(WeakPtr&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(WeakPtr<Y, ThreadSafe>&& r) noexcept { MoveFrom(std::move(r)); }

		~WeakPtr() { if (obj_) obj_->DecWeak();	}
		
		WeakPtr& operator=(const WeakPtr& r) noexcept
		{
			WeakPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr& operator=(const WeakPtr<Y, ThreadSafe>& r) noexcept
		{
			WeakPtr{r}.Swap(*this);
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
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
		WeakPtr& operator=(WeakPtr<Y, ThreadSafe>&& r) noexcept
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
		
		[[nodiscard]] SharedPtr<T, ThreadSafe> Lock() const noexcept
		{
			SharedPtr<T, ThreadSafe> ret;
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
	class EnableSharedFromThis
	{
	public:
		using EsftType = EnableSharedFromThis;
		
		[[nodiscard]] WeakPtr<T, ThreadSafe> WeakFromThis() noexcept { return weak_; }
		[[nodiscard]] WeakPtr<const T, ThreadSafe> WeakFromThis() const noexcept { return weak_; }
		[[nodiscard]] SharedPtr<T, ThreadSafe> SharedFromThis() noexcept { return SharedPtr<T, ThreadSafe>{weak_}; }
		[[nodiscard]] SharedPtr<const T, ThreadSafe> SharedFromThis() const noexcept { return SharedPtr<const T, ThreadSafe>{weak_}; }

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

	template <class T, bool ThreadSafe = OENG_SHARED_PTR_THREADSAFE, class... Args>
	SharedPtr<T, ThreadSafe> MakeShared(Args&&... args)
	{
		SharedPtr<T, ThreadSafe> ret;
		auto obj = new detail::SharedObjInline<T, ThreadSafe>{std::forward<Args>(args)...};
		ret.SetAndEnableShared(&obj->obj, obj);
		return ret;
	}

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
