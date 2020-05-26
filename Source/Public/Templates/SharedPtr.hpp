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
				if constexpr (OENG_SHARED_PTR_THREADSAFE)
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
				if constexpr (OENG_SHARED_PTR_THREADSAFE)
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
				if constexpr (OENG_SHARED_PTR_THREADSAFE)
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

				if constexpr (OENG_SHARED_PTR_THREADSAFE)
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

				if constexpr (OENG_SHARED_PTR_THREADSAFE)
				{
					old_weak = weak.fetch_sub(1, std::memory_order_acq_rel);
				}
				else
				{
					old_weak = weak--;
				}

				if (old_weak == 1)
				{
					Destroy();
					DecWeak();
				}
			}

			unsigned long Strong() const noexcept
			{
				if constexpr (OENG_SHARED_PTR_THREADSAFE)
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
				if constexpr (OENG_SHARED_PTR_THREADSAFE)
				{
					return weak.load(std::memory_order_relaxed);
				}
				else
				{
					return weak;
				}
			}

		private:
			virtual ~SharedObjBase() = default;
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

		template <class T, std::invocable<T*> Deleter>
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
		constexpr SharedPtr() noexcept :ptr_{}, obj_{} {}
		constexpr SharedPtr(nullptr_t) noexcept :ptr_{}, obj_{} {}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		explicit SharedPtr(Y* ptr) { Reset(ptr); }

		template <class Y, std::invocable<Y*> Deleter, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(Y* ptr, Deleter deleter) { Reset(ptr, std::move(deleter)); }

		SharedPtr(const SharedPtr& r) noexcept { CopyFrom(r); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const SharedPtr<Y>& r) noexcept { CopyFrom(r); }

		SharedPtr(SharedPtr&& r) noexcept { MoveFrom(std::move(r)); }
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y>&& r) noexcept { MoveFrom(std::move(r)); }

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const WeakPtr<Y>& r)
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (!obj_ || !obj_->IncStrongNz()) throw std::bad_weak_ptr{};
		}

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

		template <class Y, std::invocable<Y*> Deleter = std::default_delete<Y>, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void Reset(Y* ptr, Deleter deleter = {})
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
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void CopyFrom(const SharedPtr<Y>& r) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = r.ptr_;
			obj_ = r.obj_;
		}
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void MoveFrom(SharedPtr<Y>&& r) noexcept
		{
			ptr_ = r.ptr_;
			obj_ = r.obj_;
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}
		
		T* ptr_;
		detail::SharedObjBase* obj_;
	};

	template <class T>
	SharedPtr(WeakPtr<T>) -> SharedPtr<T>;

	template <class T>
	class WeakPtr
	{
	public:
		constexpr WeakPtr() noexcept :ptr_{}, obj_{} {}

		WeakPtr(const WeakPtr& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (obj_) obj_->IncWeak();
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const WeakPtr<Y>& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (obj_) obj_->IncWeak();
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(const SharedPtr<Y>& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (obj_) obj_->IncWeak();
		}
		
		WeakPtr(WeakPtr&& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		WeakPtr(WeakPtr<Y>&& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}
		
	private:
		T* ptr_;
		detail::SharedObjBase* obj_;
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
		friend SharedPtr<T>;
		WeakPtr<T> weak_;
	};
}

template <class T>
struct std::hash<oeng::SharedPtr<T>>
{
	size_t operator()(const oeng::SharedPtr<T>& p) const noexcept
	{
		return std::hash<T*>{}(p.Get());
	}
};
