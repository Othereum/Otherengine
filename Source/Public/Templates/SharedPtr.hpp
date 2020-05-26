#pragma once
#include <atomic>
#include <cassert>
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

			unsigned long Strong() const noexcept { return strong; }
			unsigned long Weak() const noexcept { return weak; }

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
		constexpr SharedPtr() noexcept :ptr_{}, obj_{} {}
		constexpr SharedPtr(nullptr_t) noexcept :ptr_{}, obj_{} {}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		explicit SharedPtr(Y* ptr)
			:ptr_{ptr}, obj_{new detail::SharedObjPtr<Y, std::default_delete<Y>>{ptr, {}}}
		{
			if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, Y>)
			{
				ptr->weak_ = *this;
			}
		}

		template <class Y, class Deleter, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(Y* ptr, Deleter deleter)
			:ptr_{ptr}, obj_{new detail::SharedObjPtr<Y, Deleter>{ptr, std::move(deleter)}}
		{
			if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, Y>)
			{
				ptr->weak_ = *this;
			}
		}

		SharedPtr(const SharedPtr& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (obj_) obj_->IncStrong();
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const SharedPtr<Y>& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (obj_) obj_->IncStrong();
		}

		SharedPtr(SharedPtr&& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(SharedPtr<Y>&& r) noexcept
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			r.ptr_ = nullptr;
			r.obj_ = nullptr;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr(const WeakPtr<Y>& r)
			:ptr_{r.ptr_}, obj_{r.obj_}
		{
			if (!obj_) throw std::bad_weak_ptr{};
		}

		~SharedPtr()
		{
			if (obj_) obj_->DecStrong();
		}

		SharedPtr& operator=(const SharedPtr& r) noexcept
		{
			if (this != &r)
			{
				if (obj_) obj_->DecStrong();
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				if (obj_) obj_->IncStrong();
			}
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr& operator=(const SharedPtr<Y>& r) noexcept
		{
			if (this != &r)
			{
				if (obj_) obj_->DecStrong();
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				if (obj_) obj_->IncStrong();
			}
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& r) noexcept
		{
			if (this != &r)
			{
				if (obj_) obj_->DecStrong();
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				r.ptr_ = nullptr;
				r.obj_ = nullptr;
			}
			return *this;
		}
		
		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		SharedPtr& operator=(SharedPtr<Y>&& r) noexcept
		{
			if (this != &r)
			{
				if (obj_) obj_->DecStrong();
				ptr_ = r.ptr_;
				obj_ = r.obj_;
				r.ptr_ = nullptr;
				r.obj_ = nullptr;
			}
			return *this;
		}

		template <class Y, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void Reset(Y* ptr)
		{
			if (obj_) obj_->DecStrong();
			ptr_ = ptr;
			obj_ = new detail::SharedObjPtr<Y, std::default_delete<Y>>{ptr, {}};
			if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, Y>)
			{
				ptr->weak_ = *this;
			}
		}

		template <class Y, class Deleter, std::enable_if_t<std::is_convertible_v<Y*, T*>, int> = 0>
		void Reset(Y* ptr, Deleter deleter)
		{
			if (obj_) obj_->DecStrong();
			ptr_ = ptr;
			obj_ = new detail::SharedObjPtr<Y, Deleter>{ptr, std::move(deleter)};
			if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, Y>)
			{
				ptr->weak_ = *this;
			}
		}

		[[nodiscard]] T* Get() const noexcept { return ptr_; }

		T& operator*() const noexcept { return *ptr_; }
		T* operator->() const noexcept { return ptr_; }

		[[nodiscard]] unsigned long UseCount() const noexcept { return obj_ ? obj_->Strong() : 0; }

		explicit operator bool() const noexcept { return ptr_; }
		
	private:
		T* ptr_;
		detail::SharedObjBase* obj_;
	};

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
