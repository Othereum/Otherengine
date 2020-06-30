#pragma once
#include <cassert>
#include <memory>
#include <omem.hpp>
#include "Core.hpp"
#include "Templates/Sync.hpp"

namespace oeng
{
	inline constexpr bool kThreadSafe = OE_SHARED_PTR_THREADSAFE;
	
	namespace detail
	{
		inline void CheckMemSafe() noexcept
		{
			assert(OMEM_THREADSAFE || IsGameThread());
			assert(IsEngineExists());
		}
	}
	
	[[nodiscard]] inline void* Alloc(size_t size)
	{
		detail::CheckMemSafe();
		return omem::MemoryPool::Get(size).Alloc();
	}

	inline void Free(void* p, size_t size) noexcept
	{
		detail::CheckMemSafe();
		omem::MemoryPool::Get(size).Free(p);
	}

	/**
	 * \brief Allocate memory and construct new object T.
	 * \tparam T object type to be created
	 * \param args arguments to be passed to the constructor of object T.
	 * \return The pointer to created object
	 * \throw std::bad_alloc If failed to allocate memory
	 * \throw std::exception Any exceptions thrown by constructor of T
	 */
	template <class T, class... Args>
	[[nodiscard]] T* New(Args&&... args)
	{
		auto* const p = Alloc(sizeof(T));
		if (!p) throw std::bad_alloc{};

		try
		{
			return new (p) T{std::forward<Args>(args)...};
		}
		catch (...)
		{
			Free(p, sizeof(T));
			throw;
		}
	}

	template <class T, class... Args>
	[[nodiscard]] T* NewArr(size_t n, Args&&... args)
	{
		const auto p = Alloc(n * sizeof(T));
		if (!p) throw std::bad_alloc{};

		try
		{
			return new (p) T[n]{std::forward<Args>(args)...};
		}
		catch (...)
		{
			Free(p, n * sizeof(T));
			throw;
		}
	}

	template <class T>
	void Delete(T* p) noexcept
	{
		p->~T();
		Free(p, sizeof(T));
	}

	template <class T>
	void DeleteArr(T* p, size_t n) noexcept
	{
		for (size_t i=0; i<n; ++i) p[i].~T();
		Free(p, n * sizeof(T));
	}

	template <class T>
	using RawAllocator = std::allocator<T>;

	/**
	 * \brief Allocator for memory pool
	 * \note Allocates from pool only for a single object and game thread.
	 *		Otherwise, allocates by operator new()
	 * \tparam T Type to be allocated
	 */
	template <class T>
	class PoolAllocator
	{
	public:
		using value_type = T;
		
		constexpr PoolAllocator() noexcept = default;

		template <class Y>
		constexpr PoolAllocator(const PoolAllocator<Y>&) noexcept {}

		template <class Y>
		constexpr bool operator==(const PoolAllocator<Y>&) const noexcept { return true; }

		/**
		 * \brief Allocate n * sizeof(T) bytes of uninitialized memory.
		 * \param n the number of objects to allocate memory for
		 * \return The pointer to allocated memory
		 * \note Allocates from pool only if IsGameThread()
		 */
		// ReSharper disable once CppMemberFunctionMayBeStatic
		[[nodiscard]] T* allocate(size_t n) const
		{
			return static_cast<T*>(Alloc(n * sizeof(T)));
		}

		/**
		 * \brief Deallocate the memory referenced by p
		 * \param p pointer to the previously allocated memory
		 * \param n the number of objects the storage was allocated for (MUST BE SAME)
		 */
		// ReSharper disable once CppMemberFunctionMayBeStatic
		void deallocate(T* p, size_t n) const noexcept
		{
			Free(p, n * sizeof(T));
		}
	};

	template <class T>
	using RawDeleter = std::default_delete<T>;

	template <class T>
	class PoolDeleter
	{
	public:
		static_assert(!std::is_array_v<T>, "Can't delete array because size is unknown. Use RawDeleter instead.");
		void operator()(T* p) noexcept { Delete(p); }
	};

	template <class T, class Deleter = PoolDeleter<T>>
	using UniquePtr = std::unique_ptr<T, Deleter>;

	template <class T, class... Args, std::enable_if_t<!std::is_array_v<T>, int> = 0>
	[[nodiscard]] UniquePtr<T> MakeUnique(Args&&... args)
	{
	    return UniquePtr<T>(New<T>(std::forward<Args>(args)...));
	}

	template <class T, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, int> = 0>
	[[nodiscard]] UniquePtr<T> MakeUnique(size_t size)
	{
	    using Elem = std::remove_extent_t<T>;
	    return UniquePtr<T>(NewArr<Elem>(size)());
	}

	template <class T, class... Args, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
	void MakeUnique(Args&&...) = delete;

	template <class T, bool ThreadSafe = kThreadSafe>
	class SharedPtr;
	
	namespace detail
	{
		template <bool ThreadSafe>
		// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
		struct SharedObjBase
		{
			bool IncStrongNz() noexcept
			{
				auto count = strong.load();
				while (count != 0)
					if (strong.compare_exchange_strong(count, count+1, std::memory_order_relaxed))
						return true;
				return false;
			}

			void IncStrong() noexcept
			{
				strong.fetch_add(1, std::memory_order_relaxed);
			}

			void IncWeak() noexcept
			{
				weak.fetch_add(1, std::memory_order_relaxed);
			}

			void DecStrong() noexcept
			{
				if (1 == strong.fetch_sub(1, std::memory_order_acq_rel))
				{
					Destroy();
					DecWeak();
				}
			}

			void DecWeak() noexcept
			{
				if (1 == weak.fetch_sub(1, std::memory_order_acq_rel))
				{
					DeleteThis();
				}
			}

			[[nodiscard]] unsigned long Strong() const noexcept
			{
				return strong.load(std::memory_order_relaxed);
			}

			[[nodiscard]] unsigned long Weak() const noexcept
			{
				return weak.load(std::memory_order_relaxed);
			}

			virtual void Destroy() noexcept = 0;
			virtual void DeleteThis() noexcept = 0;

			using RefCnt = CondAtomic<unsigned, ThreadSafe>;
			RefCnt strong = 1;
			RefCnt weak = 1;
		};

		template <class T, class Alloc, bool ThreadSafe>
		// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
		struct SharedObjInline : SharedObjBase<ThreadSafe>  // NOLINT(cppcoreguidelines-special-member-functions)
		{
			template <class... Args>
			SharedObjInline(Alloc alloc, Args&&... args)
				:obj{ std::forward<Args>(args)... }, alloc{std::move(alloc)}
			{
			}

			~SharedObjInline() {}

			void Destroy() noexcept override
			{
				std::allocator_traits<Alloc>::destroy(alloc, &obj);
			}
			
			void DeleteThis() noexcept override
			{
				using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<SharedObjInline>;
				using Tr = std::allocator_traits<Al>;
				alloc.~Alloc();
				Al al{std::move(alloc)};
				Tr::deallocate(al, this, 1);
			}

			union { T obj; };
			[[no_unique_address]] Alloc alloc;
		};

		template <class T, class Deleter, class Alloc, bool ThreadSafe>
		// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
		struct SharedObjPtr : SharedObjBase<ThreadSafe>
		{
			SharedObjPtr(T* ptr, Deleter deleter, Alloc alloc) noexcept
				:ptr{ptr}, deleter{std::move(deleter)}, alloc{std::move(alloc)}
			{
			}

			void Destroy() noexcept override { deleter(ptr); }
			void DeleteThis() noexcept override
			{
				using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<SharedObjPtr>;
				using Tr = std::allocator_traits<Al>;
				alloc.~Alloc();
				deleter.~Deleter();
				Al al{std::move(alloc)};
				Tr::deallocate(al, this, 1);
			}

			T* ptr;
			[[no_unique_address]] Deleter deleter;
			[[no_unique_address]] Alloc alloc;
		};

		template <class T, class = void>
		struct CanEnableShared : std::false_type {};

		template <class T>
		struct CanEnableShared<T, std::void_t<typename T::EsftType>>
			: std::is_convertible<std::remove_cv_t<T>*, typename T::EsftType*>::type
		{
		};

		template <class T, bool ThreadSafe>
		struct SetAndEnableShared
		{
			template <class Y>
			SetAndEnableShared(SharedPtr<T, ThreadSafe>& sp, Y* rp, SharedObjBase<ThreadSafe>* obj) noexcept
			{
				sp.SetAndEnableShared(rp, obj);
			}
		};		
	}

	template <class T, bool ThreadSafe = kThreadSafe>
	class SharedRef;
	
	template <class T, bool ThreadSafe = kThreadSafe>
	class EnableSharedFromThis;

	template <class T, bool ThreadSafe = kThreadSafe>
	class WeakPtr;

	template <class T, bool ThreadSafe>
	class SharedPtr
	{
	public:
		using element_type = std::remove_extent_t<T>;
		using weak_type = WeakPtr<T, ThreadSafe>;
		
		constexpr SharedPtr() noexcept = default;
		constexpr SharedPtr(std::nullptr_t) noexcept {}

		/**
		 * \brief Construct with raw pointer
		 * \param ptr Must be allocated by New<T>() because it uses PoolDeleter<Y> to delete ptr
		 * \note Only participates in overload if T is not array
		 */
		template <class Y, class Deleter = std::enable_if_t<!std::is_array_v<T>, PoolDeleter<Y>>>
		explicit SharedPtr(Y* ptr)
			:SharedPtr{ptr, Deleter{}}
		{
		}

		/**
		 * \brief Construct with raw pointer and custom deleter/allocator
		 * \param ptr Raw pointer
		 * \param deleter To be used to delete ptr
		 * \param alloc To be used to allocate/deallocate control block
		 */
		template <class Y, class Deleter, class Alloc = PoolAllocator<Y>>
		SharedPtr(Y* ptr, Deleter deleter, Alloc alloc = {})
		{
			using Obj = detail::SharedObjPtr<Y, Deleter, Alloc, ThreadSafe>;
			using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<Obj>;
			using Tr = std::allocator_traits<Al>;

			Al al{alloc};
			auto obj = Tr::allocate(al, 1);
			Tr::construct(al, obj, ptr, std::move(deleter), std::move(alloc));
			
			SetAndEnableShared(ptr, obj);
		}

		template <class Y>
		SharedPtr(const SharedPtr<Y, ThreadSafe>& r, element_type* ptr) noexcept { AliasCopyFrom(r, ptr); }

		template <class Y>
		SharedPtr(SharedPtr<Y, ThreadSafe>&& r, element_type* ptr) noexcept { AliasMoveFrom(std::move(r), ptr); }

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

		void reset(std::nullptr_t) noexcept { reset(); }

		template <class Y>
		void reset(Y* ptr)
		{
			SharedPtr{ptr}.swap(*this);
		}

		template <class Y, class Deleter>
		void reset(Y* ptr, Deleter deleter)
		{
			SharedPtr{ptr, std::move(deleter)}.swap(*this);
		}

		template <class Y, class Deleter, class Alloc>
		void reset(Y* ptr, Deleter deleter, Alloc alloc)
		{
			SharedPtr{ptr, std::move(deleter), std::move(alloc)}.swap(*this);
		}

		void swap(SharedPtr& r) noexcept
		{
			using std::swap;
			swap(ptr_, r.ptr_);
			swap(obj_, r.obj_);
		}

		[[nodiscard]] element_type* get() const noexcept { return ptr_; }

		template <class T2 = T, std::enable_if_t<!std::is_void_v<T2>, int> = 0>
		T2& operator*() const noexcept { assert(ptr_); return *ptr_; }
		
		template <class T2 = T, std::enable_if_t<!std::is_void_v<T2>, int> = 0>
		T2* operator->() const noexcept { assert(ptr_); return ptr_; }
		
		template <class T2 = T, class Elem = element_type, std::enable_if_t<!std::is_void_v<T2>, int> = 0>
		Elem& operator[](ptrdiff_t idx) const noexcept { return ptr_[idx]; }

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

		constexpr bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }
		constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr_ != nullptr; }
		constexpr bool operator<(std::nullptr_t) const noexcept { return ptr_ < nullptr; }
		constexpr bool operator>(std::nullptr_t) const noexcept { return ptr_ > nullptr; }
		constexpr bool operator<=(std::nullptr_t) const noexcept { return ptr_ <= nullptr; }
		constexpr bool operator>=(std::nullptr_t) const noexcept { return ptr_ >= nullptr; }

	private:
		template <class, bool>
		friend class SharedPtr;

		template <class, bool>
		friend class WeakPtr;

		template <class, bool>
		friend struct detail::SetAndEnableShared;

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
		void AliasCopyFrom(const SharedPtr<Y, ThreadSafe>& r, element_type* ptr) noexcept
		{
			if (r.obj_) r.obj_->IncStrong();
			ptr_ = ptr;
			obj_ = r.obj_;
		}

		template <class Y>
		void AliasMoveFrom(SharedPtr<Y, ThreadSafe>&& r, element_type* ptr) noexcept
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
					assert(ptr_->weak_.expired());
					ptr->weak_ = SharedPtr<std::remove_cv_t<Y>, ThreadSafe>{*this, const_cast<std::remove_cv_t<Y>*>(ptr)};
				}
			}
		}

		element_type* ptr_ = nullptr;
		detail::SharedObjBase<ThreadSafe>* obj_ = nullptr;
	};

	template <class T, bool ThreadSafe>
	class SharedRef : public SharedPtr<T, ThreadSafe>
	{
		using Base = SharedPtr<T, ThreadSafe>;
		
	public:
		using typename Base::element_type;
		using typename Base::weak_type;
		
		SharedRef(std::nullptr_t) = delete;
		
		/**
		 * \brief Construct SharedRef with raw pointer.
		 * \param ptr Must not be nullptr
		 * \throw std::invalid_argument If ptr is nullptr
		 */
		template <class Y>
		explicit SharedRef(Y* ptr)
			:Base{ThrowIfNull(ptr)}
		{
		}

		/**
		 * \brief Construct SharedRef with raw pointer and custom deleter/allocator
		 * \param ptr Must not be nullptr
		 * \param deleter To be used to delete ptr
		 * \param alloc To be used to allocate/deallocate control block
		 * \throw std::invalid_argument If ptr is nullptr
		 */
		template <class Y, class Deleter, class Alloc = PoolAllocator<Y>>
		SharedRef(Y* ptr, Deleter deleter, Alloc alloc = {})
			:Base{ThrowIfNull(ptr), std::move(deleter), std::move(alloc)}
		{
		}

		template <class Y>
		SharedRef(const SharedRef<Y>& r, element_type* p) noexcept
			:Base{r, ThrowIfNull(p)}
		{
		}

		template <class Y>
		SharedRef(SharedRef<Y>&& r, element_type* p) noexcept
			:Base{std::move(r), ThrowIfNull(p)}
		{
		}

		template <class Y>
		SharedRef(const SharedRef<Y, ThreadSafe>& r) noexcept
			:Base{r}
		{
		}

		template <class Y>
		SharedRef(SharedRef<Y, ThreadSafe>&& r) noexcept
			:Base{std::move(r)}
		{
		}

		/**
		 * \brief Construct from WeakPtr
		 * \param weak Must not be (expired or nullptr)
		 * \throw std::bad_weak_ptr if weak is expired
		 * \throw std::invalid_argument if weak is nullptr
		 */
		template <class Y>
		explicit SharedRef(const WeakPtr<Y, ThreadSafe>& weak)
			:Base{weak}
		{
			ThrowIfNull(this->get());
		}

		/**
		 * \brief Construct from SharedPtr
		 * \param p Must not be nullptr
		 * \throw std::invalid_argument if p is nullptr
		 */
		template <class Y>
		explicit SharedRef(const SharedPtr<Y, ThreadSafe>& p)
			:Base{ThrowIfNull(p)}
		{
		}

		/**
		 * \brief Construct from SharedPtr
		 * \param p Must not be nullptr
		 * \throw std::invalid_argument if p is nullptr
		 */
		template <class Y>
		explicit SharedRef(SharedPtr<Y, ThreadSafe>&& p)
			:Base{ThrowIfNull(std::move(p))}
		{
		}

		template <class Y>
		SharedRef& operator=(const SharedRef<Y, ThreadSafe>& r)
		{
			Base::operator=(r);
			return *this;
		}

		template <class Y>
		SharedRef& operator=(SharedRef<Y, ThreadSafe>&& r)
		{
			Base::operator=(std::move(r));
			return *this;
		}

		void reset(std::nullptr_t) = delete;

		template <class Y>
		void reset(Y* ptr)
		{
			ThrowIfNull(ptr);
			Base::reset(ptr);
		}

		template <class Y, class Deleter>
		void reset(Y* ptr, Deleter deleter)
		{
			ThrowIfNull(ptr);
			Base::reset(ptr, std::move(deleter));
		}

		template <class Y, class Deleter, class Alloc>
		void reset(Y* ptr, Deleter deleter, Alloc alloc)
		{
			ThrowIfNull(ptr);
			Base::reset(ptr, std::move(deleter), std::move(alloc));
		}
		
	private:
		template <class Ptr>
		static Ptr&& ThrowIfNull(Ptr&& p)
		{
			if (!p) throw std::invalid_argument{"SharedRef cannot be null"};
			return std::forward<Ptr>(p);
		}
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
		[[nodiscard]] SharedRef<T, ThreadSafe> shared_from_this() { return SharedRef<T, ThreadSafe>{weak_}; }
		[[nodiscard]] SharedRef<const T, ThreadSafe> shared_from_this() const { return SharedRef<const T, ThreadSafe>{weak_}; }

	protected:
		constexpr EnableSharedFromThis() noexcept :weak_{} {}
		EnableSharedFromThis(const EnableSharedFromThis&) noexcept :weak_{} {}
		EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept { return *this; }
		~EnableSharedFromThis() = default;

	private:
		template <class, bool>
		friend class SharedPtr;

		template <class, class, bool>
		friend struct detail::SharedObjInline;
		
		mutable WeakPtr<T, ThreadSafe> weak_;
	};

	template <class T, bool ThreadSafe = kThreadSafe, std::enable_if_t<!std::is_array_v<T>, int> = 0, class Alloc, class... Args>
	SharedRef<T, ThreadSafe> AllocateShared(const Alloc& alloc, Args&&... args)
	{
		using Obj = detail::SharedObjInline<T, Alloc, ThreadSafe>;
		using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<Obj>;
		using Tr = std::allocator_traits<Al>;

		Al al{alloc};
		auto* const obj = Tr::allocate(al, 1);
		try
		{
			Tr::construct(al, obj, alloc, std::forward<Args>(args)...);
			try
			{
				SharedPtr<T, ThreadSafe> ret;
				detail::SetAndEnableShared<T, ThreadSafe>(ret, &obj->obj, obj);
				return SharedRef<T, ThreadSafe>{std::move(ret)};
			}
			catch (...)
			{
				obj->DeleteThis();
				throw;
			}
		}
		catch (...)
		{
			Tr::deallocate(al, obj, 1);
			throw;
		}
	}

	template <class T, bool ThreadSafe = kThreadSafe, std::enable_if_t<std::is_array_v<T>, int> = 0, class Alloc>
	SharedRef<T, ThreadSafe> AllocateShared(const Alloc& alloc, size_t n)
	{
		using Tr = std::allocator_traits<Alloc>;

		auto* const ptr = Tr::allocate(alloc, n);
		size_t idx = 0;
		try
		{
			for (; idx < n; ++idx) Tr::construct(alloc, ptr + idx);
			
			auto deleter = [=](std::remove_extent_t<T>* p) noexcept
			{
				for (size_t i=0; i<n; ++i) Tr::destroy(alloc, p+i);
				Tr::deallocate(alloc, p, n);
			};
			
			return {ptr, std::move(deleter), alloc};
		}
		catch (...)
		{
			for (; idx > 0; --idx) Tr::destroy(alloc, ptr + (idx - 1));
			Tr::deallocate(alloc, ptr, n);
			throw;
		}
	}

	template <class T, bool ThreadSafe = kThreadSafe, class... Args,
		std::enable_if_t<!std::is_array_v<T>, int> = 0>
	SharedRef<T, ThreadSafe> MakeShared(Args&&... args)
	{
		return AllocateShared<T, ThreadSafe>(PoolAllocator<T>{}, std::forward<Args>(args)...);
	}

	template <class T, bool ThreadSafe = kThreadSafe,
		std::enable_if_t<std::is_array_v<T>, int> = 0>
	SharedRef<T, ThreadSafe> MakeShared(size_t n)
	{
		return AllocateShared<T, ThreadSafe>(PoolAllocator<std::remove_extent_t<T>>{}, n);
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
	constexpr bool operator==(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p == nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator!=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p != nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator<(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p < nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator>(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p > nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator<=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p <= nullptr; }

	template <class T, bool ThreadSafe>
	constexpr bool operator>=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept { return p >= nullptr; }

	template <class T, bool ThreadSafe>
	void swap(SharedPtr<T, ThreadSafe>& l, SharedPtr<T, ThreadSafe>& r) noexcept { l.swap(r); }

	template <class T, bool ThreadSafe>
	void swap(WeakPtr<T, ThreadSafe>& l, WeakPtr<T, ThreadSafe>& r) noexcept { l.swap(r); }

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
