#pragma once
#include "CompPair.hpp"
#include "Sync.hpp"
#include <cassert>
#include <memory>

namespace oeng
{
inline namespace core
{

template <class T, bool ThreadSafe = false> class SharedPtr;

namespace detail
{

template <bool ThreadSafe> struct SharedObjBase
{
    bool IncStrongNz() noexcept
    {
        auto count = strong.load();
        while (count != 0)
            if (strong.compare_exchange_strong(count, count + 1, std::memory_order_relaxed))
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

template <class T, class Alloc, bool ThreadSafe> struct SharedObjInline : SharedObjBase<ThreadSafe>
{
    template <class... Args>
    SharedObjInline(Alloc alloc, Args&&... args) : storage{OneThen{}, std::move(alloc), std::forward<Args>(args)...}
    {
    }

    ~SharedObjInline()
    {
    }

    void Destroy() noexcept override
    {
        std::allocator_traits<Alloc>::destroy(storage.first(), &storage.second());
    }

    void DeleteThis() noexcept override
    {
        using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<SharedObjInline>;
        using Tr = std::allocator_traits<Al>;
        storage.first().~Alloc();
        Al al{std::move(storage.first())};
        Tr::deallocate(al, this, 1);
    }

    union {
        CompPair<Alloc, T> storage;
    };
};

template <class T, class Deleter, class Alloc, bool ThreadSafe> struct SharedObjPtr : SharedObjBase<ThreadSafe>
{
    SharedObjPtr(T* ptr, Deleter deleter, Alloc alloc) noexcept
        : storage{OneThen{}, std::move(alloc), OneThen{}, std::move(deleter), ptr}
    {
    }

    void Destroy() noexcept override
    {
        storage.second().first()(storage.second().second());
    }

    void DeleteThis() noexcept override
    {
        using Al = typename std::allocator_traits<Alloc>::template rebind_alloc<SharedObjPtr>;
        using Tr = std::allocator_traits<Al>;
        storage.first().~Alloc();
        storage.second().first().~Deleter();
        Al al{std::move(storage.first())};
        Tr::deallocate(al, this, 1);
    }

    CompPair<Alloc, CompPair<Deleter, T*>> storage;
};

template <class T, class = void> struct CanEnableShared : std::false_type
{
};

template <class T>
struct CanEnableShared<T, std::void_t<typename T::EnableSharedFromThisType>>
    : std::is_convertible<std::remove_cv_t<T>*, typename T::EnableSharedFromThisType*>::type
{
};

template <class T, bool ThreadSafe> struct SetAndEnableShared
{
    template <class Y> SetAndEnableShared(SharedPtr<T, ThreadSafe>& sp, Y* rp, SharedObjBase<ThreadSafe>* obj) noexcept
    {
        sp.SetAndEnableShared(rp, obj);
    }
};

} // namespace detail

template <class T, bool ThreadSafe = false> class SharedRef;
template <class T, bool ThreadSafe = false> class EnableSharedFromThis;
template <class T, bool ThreadSafe = false> class WeakPtr;

template <class T, bool ThreadSafe> class SharedPtr
{
  public:
    using element_type = std::remove_extent_t<T>;
    using weak_type = WeakPtr<T, ThreadSafe>;

    constexpr SharedPtr() noexcept = default;

    constexpr SharedPtr(std::nullptr_t) noexcept
    {
    }

    /**
     * Construct with raw pointer
     * @param ptr Raw pointer
     */
    template <class Y> explicit SharedPtr(Y* ptr) : SharedPtr{ptr, std::default_delete<Y>{}}
    {
    }

    /**
     * Construct with raw pointer and custom deleter/allocator
     * @param ptr Raw pointer
     * @param deleter To be used to delete ptr
     * @param alloc To be used to allocate/deallocate control block
     */
    template <class Y, class Deleter, class Alloc = std::allocator<Y>>
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

    template <class Y> SharedPtr(const SharedPtr<Y, ThreadSafe>& r, element_type* ptr) noexcept
    {
        AliasCopyFrom(r, ptr);
    }

    template <class Y> SharedPtr(SharedPtr<Y, ThreadSafe>&& r, element_type* ptr) noexcept
    {
        AliasMoveFrom(std::move(r), ptr);
    }

    SharedPtr(const SharedPtr& r) noexcept
    {
        CopyFrom(r);
    }

    template <class Y> SharedPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept
    {
        CopyFrom(r);
    }

    SharedPtr(SharedPtr&& r) noexcept
    {
        MoveFrom(std::move(r));
    }

    template <class Y> SharedPtr(SharedPtr<Y, ThreadSafe>&& r) noexcept
    {
        MoveFrom(std::move(r));
    }

    template <class Y> explicit SharedPtr(const WeakPtr<Y, ThreadSafe>& r)
    {
        if (!FromWeak(r))
            throw std::bad_weak_ptr{};
    }

    ~SharedPtr()
    {
        reset();
    }

    SharedPtr& operator=(const SharedPtr& r) noexcept
    {
        SharedPtr{r}.swap(*this);
        return *this;
    }

    template <class Y> SharedPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
    {
        SharedPtr{r}.swap(*this);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& r) noexcept
    {
        SharedPtr{std::move(r)}.swap(*this);
        return *this;
    }

    template <class Y> SharedPtr& operator=(SharedPtr<Y, ThreadSafe>&& r) noexcept
    {
        SharedPtr{std::move(r)}.swap(*this);
        return *this;
    }

    void reset() noexcept
    {
        if (obj_)
            obj_->DecStrong();
        obj_ = nullptr;
        ptr_ = nullptr;
    }

    void reset(std::nullptr_t) noexcept
    {
        reset();
    }

    template <class Y> void reset(Y* ptr)
    {
        SharedPtr{ptr}.swap(*this);
    }

    template <class Y, class Deleter> void reset(Y* ptr, Deleter deleter)
    {
        SharedPtr{ptr, std::move(deleter)}.swap(*this);
    }

    template <class Y, class Deleter, class Alloc> void reset(Y* ptr, Deleter deleter, Alloc alloc)
    {
        SharedPtr{ptr, std::move(deleter), std::move(alloc)}.swap(*this);
    }

    void swap(SharedPtr& r) noexcept
    {
        using std::swap;
        swap(ptr_, r.ptr_);
        swap(obj_, r.obj_);
    }

    [[nodiscard]] element_type* get() const noexcept
    {
        return ptr_;
    }

    template <class T2 = T, std::enable_if_t<!std::is_void_v<T2>, int> = 0> T2& operator*() const noexcept
    {
        assert(ptr_);
        return *ptr_;
    }

    template <class T2 = T, std::enable_if_t<!std::is_void_v<T2>, int> = 0> T2* operator->() const noexcept
    {
        assert(ptr_);
        return ptr_;
    }

    template <class T2 = T, class Elem = element_type, std::enable_if_t<!std::is_void_v<T2>, int> = 0>
    Elem& operator[](ptrdiff_t idx) const noexcept
    {
        return ptr_[idx];
    }

    [[nodiscard]] unsigned long use_count() const noexcept
    {
        return obj_ ? obj_->Strong() : 0;
    }

    explicit operator bool() const noexcept
    {
        return ptr_;
    }

    template <class U> constexpr bool operator==(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ == r.ptr_;
    }

    template <class U> constexpr bool operator!=(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ != r.ptr_;
    }

    template <class U> constexpr bool operator<(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ < r.ptr_;
    }

    template <class U> constexpr bool operator>(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ > r.ptr_;
    }

    template <class U> constexpr bool operator<=(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ <= r.ptr_;
    }

    template <class U> constexpr bool operator>=(const SharedPtr<U, ThreadSafe>& r) const noexcept
    {
        return ptr_ >= r.ptr_;
    }

    constexpr bool operator==(std::nullptr_t) const noexcept
    {
        return ptr_ == nullptr;
    }

    constexpr bool operator!=(std::nullptr_t) const noexcept
    {
        return ptr_ != nullptr;
    }

    constexpr bool operator<(std::nullptr_t) const noexcept
    {
        return ptr_ < nullptr;
    }

    constexpr bool operator>(std::nullptr_t) const noexcept
    {
        return ptr_ > nullptr;
    }

    constexpr bool operator<=(std::nullptr_t) const noexcept
    {
        return ptr_ <= nullptr;
    }

    constexpr bool operator>=(std::nullptr_t) const noexcept
    {
        return ptr_ >= nullptr;
    }

  private:
    template <class, bool> friend class SharedPtr;

    template <class, bool> friend class WeakPtr;

    template <class, bool> friend struct detail::SetAndEnableShared;

    template <class Y> void CopyFrom(const SharedPtr<Y, ThreadSafe>& r) noexcept
    {
        if (r.obj_)
            r.obj_->IncStrong();
        ptr_ = r.ptr_;
        obj_ = r.obj_;
    }

    template <class Y> void MoveFrom(SharedPtr<Y, ThreadSafe>&& r) noexcept
    {
        ptr_ = r.ptr_;
        obj_ = r.obj_;
        r.ptr_ = nullptr;
        r.obj_ = nullptr;
    }

    template <class Y> void AliasCopyFrom(const SharedPtr<Y, ThreadSafe>& r, element_type* ptr) noexcept
    {
        if (r.obj_)
            r.obj_->IncStrong();
        ptr_ = ptr;
        obj_ = r.obj_;
    }

    template <class Y> void AliasMoveFrom(SharedPtr<Y, ThreadSafe>&& r, element_type* ptr) noexcept
    {
        ptr_ = ptr;
        obj_ = r.obj_;
        r.ptr_ = nullptr;
        r.obj_ = nullptr;
    }

    template <class Y> bool FromWeak(const WeakPtr<Y, ThreadSafe>& r) noexcept
    {
        if (r.obj_ && r.obj_->IncStrongNz())
        {
            ptr_ = r.ptr_;
            obj_ = r.obj_;
            return true;
        }
        return false;
    }

    template <class Y> void SetAndEnableShared(Y* ptr, detail::SharedObjBase<ThreadSafe>* obj) noexcept
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

template <class T, bool ThreadSafe> class SharedRef : public SharedPtr<T, ThreadSafe>
{
    using Base = SharedPtr<T, ThreadSafe>;

  public:
    using typename Base::element_type;
    using typename Base::weak_type;

    SharedRef(std::nullptr_t) = delete;

    /**
     * Construct SharedRef with raw pointer.
     * @param ptr Must not be nullptr
     * @throw std::invalid_argument If ptr is nullptr
     */
    template <class Y> explicit SharedRef(Y* ptr) : Base{ThrowIfNull(ptr)}
    {
    }

    /**
     * Construct SharedRef with raw pointer and custom deleter/allocator
     * @param ptr Must not be nullptr
     * @param deleter To be used to delete ptr
     * @param alloc To be used to allocate/deallocate control block
     * @throw std::invalid_argument If ptr is nullptr
     */
    template <class Y, class Deleter, class Alloc = std::allocator<Y>>
    SharedRef(Y* ptr, Deleter deleter, Alloc alloc = {}) : Base{ThrowIfNull(ptr), std::move(deleter), std::move(alloc)}
    {
    }

    template <class Y> SharedRef(const SharedRef<Y>& r, element_type* p) noexcept : Base{r, ThrowIfNull(p)}
    {
    }

    template <class Y> SharedRef(SharedRef<Y>&& r, element_type* p) noexcept : Base{std::move(r), ThrowIfNull(p)}
    {
    }

    template <class Y> SharedRef(const SharedRef<Y, ThreadSafe>& r) noexcept : Base{r}
    {
    }

    template <class Y> SharedRef(SharedRef<Y, ThreadSafe>&& r) noexcept : Base{std::move(r)}
    {
    }

    /**
     * Construct from WeakPtr
     * @param weak Must not be (expired or nullptr)
     * @throw std::bad_weak_ptr if weak is expired
     * @throw std::invalid_argument if weak is nullptr
     */
    template <class Y> explicit SharedRef(const WeakPtr<Y, ThreadSafe>& weak) : Base{weak}
    {
        ThrowIfNull(this->get());
    }

    /**
     * Construct from SharedPtr
     * @param p Must not be nullptr
     * @throw std::invalid_argument if p is nullptr
     */
    template <class Y> explicit SharedRef(const SharedPtr<Y, ThreadSafe>& p) : Base{ThrowIfNull(p)}
    {
    }

    /**
     * Construct from SharedPtr
     * @param p Must not be nullptr
     * @throw std::invalid_argument if p is nullptr
     */
    template <class Y> explicit SharedRef(SharedPtr<Y, ThreadSafe>&& p) : Base{ThrowIfNull(std::move(p))}
    {
    }

    template <class Y> SharedRef& operator=(const SharedRef<Y, ThreadSafe>& r)
    {
        Base::operator=(r);
        return *this;
    }

    template <class Y> SharedRef& operator=(SharedRef<Y, ThreadSafe>&& r)
    {
        Base::operator=(std::move(r));
        return *this;
    }

    void reset(std::nullptr_t) = delete;

    template <class Y> void reset(Y* ptr)
    {
        ThrowIfNull(ptr);
        Base::reset(ptr);
    }

    template <class Y, class Deleter> void reset(Y* ptr, Deleter deleter)
    {
        ThrowIfNull(ptr);
        Base::reset(ptr, std::move(deleter));
    }

    template <class Y, class Deleter, class Alloc> void reset(Y* ptr, Deleter deleter, Alloc alloc)
    {
        ThrowIfNull(ptr);
        Base::reset(ptr, std::move(deleter), std::move(alloc));
    }

  private:
    template <class Ptr> static Ptr&& ThrowIfNull(Ptr&& p)
    {
        if (!p)
            throw std::invalid_argument{"SharedRef cannot be null"};

        return std::forward<Ptr>(p);
    }
};

template <class T, bool ThreadSafe> class WeakPtr
{
  public:
    constexpr WeakPtr() noexcept = default;

    WeakPtr(const WeakPtr& r) noexcept
    {
        CopyFrom(r);
    }

    template <class Y> WeakPtr(const WeakPtr<Y, ThreadSafe>& r) noexcept
    {
        CopyFrom(r);
    }

    template <class Y> WeakPtr(const SharedPtr<Y, ThreadSafe>& r) noexcept
    {
        CopyFrom(r);
    }

    WeakPtr(WeakPtr&& r) noexcept
    {
        MoveFrom(std::move(r));
    }

    template <class Y> WeakPtr(WeakPtr<Y, ThreadSafe>&& r) noexcept
    {
        MoveFrom(std::move(r));
    }

    ~WeakPtr()
    {
        if (obj_)
            obj_->DecWeak();
    }

    WeakPtr& operator=(const WeakPtr& r) noexcept
    {
        WeakPtr{r}.swap(*this);
        return *this;
    }

    template <class Y> WeakPtr& operator=(const WeakPtr<Y, ThreadSafe>& r) noexcept
    {
        WeakPtr{r}.swap(*this);
        return *this;
    }

    template <class Y> WeakPtr& operator=(const SharedPtr<Y, ThreadSafe>& r) noexcept
    {
        WeakPtr{r}.swap(*this);
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& r) noexcept
    {
        WeakPtr{std::move(r)}.swap(*this);
        return *this;
    }

    template <class Y> WeakPtr& operator=(WeakPtr<Y, ThreadSafe>&& r) noexcept
    {
        WeakPtr{std::move(r)}.swap(*this);
        return *this;
    }

    void reset() noexcept
    {
        if (obj_)
            obj_->DecWeak();
        ptr_ = nullptr;
        obj_ = nullptr;
    }

    void swap(WeakPtr& r) noexcept
    {
        using std::swap;
        swap(ptr_, r.ptr_);
        swap(obj_, r.obj_);
    }

    [[nodiscard]] unsigned long use_count() const noexcept
    {
        return obj_ ? obj_->Strong() : 0;
    }

    [[nodiscard]] bool expired() const noexcept
    {
        return use_count() == 0;
    }

    [[nodiscard]] SharedPtr<T, ThreadSafe> lock() const noexcept
    {
        SharedPtr<T, ThreadSafe> ret;
        ret.FromWeak(*this);
        return ret;
    }

  private:
    template <class, bool> friend class SharedPtr;

    template <class, bool> friend class WeakPtr;

    template <class Ptr> void CopyFrom(const Ptr& r) noexcept
    {
        if (r.obj_)
            r.obj_->IncWeak();
        ptr_ = r.ptr_;
        obj_ = r.obj_;
    }

    template <class Y> void MoveFrom(WeakPtr<Y, ThreadSafe>&& r) noexcept
    {
        ptr_ = r.ptr_;
        obj_ = r.obj_;
        r.ptr_ = nullptr;
        r.obj_ = nullptr;
    }

    T* ptr_ = nullptr;
    detail::SharedObjBase<ThreadSafe>* obj_ = nullptr;
};

template <class T, bool ThreadSafe> class EnableSharedFromThis
{
  public:
    using EnableSharedFromThisType = EnableSharedFromThis;

    [[nodiscard]] WeakPtr<T, ThreadSafe> weak_from_this() noexcept
    {
        return weak_;
    }

    [[nodiscard]] WeakPtr<const T, ThreadSafe> weak_from_this() const noexcept
    {
        return weak_;
    }

    [[nodiscard]] SharedRef<T, ThreadSafe> shared_from_this()
    {
        return SharedRef<T, ThreadSafe>{weak_};
    }

    [[nodiscard]] SharedRef<const T, ThreadSafe> shared_from_this() const
    {
        return SharedRef<const T, ThreadSafe>{weak_};
    }

  protected:
    constexpr EnableSharedFromThis() noexcept : weak_{}
    {
    }

    EnableSharedFromThis(const EnableSharedFromThis&) noexcept : weak_{}
    {
    }

    EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept
    {
        return *this;
    }

    ~EnableSharedFromThis() = default;

  private:
    template <class, bool> friend class SharedPtr;

    template <class, class, bool> friend struct detail::SharedObjInline;

    mutable WeakPtr<T, ThreadSafe> weak_;
};

template <class T, bool ThreadSafe = false, std::enable_if_t<!std::is_array_v<T>, int> = 0, class Alloc, class... Args>
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
            detail::SetAndEnableShared<T, ThreadSafe>(ret, &obj->storage.second(), obj);
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

template <class T, bool ThreadSafe = false, std::enable_if_t<std::is_array_v<T>, int> = 0, class Alloc>
SharedRef<T, ThreadSafe> AllocateShared(const Alloc& alloc, size_t n)
{
    using Tr = std::allocator_traits<Alloc>;

    auto* const ptr = Tr::allocate(alloc, n);
    size_t idx = 0;
    try
    {
        for (; idx < n; ++idx)
            Tr::construct(alloc, ptr + idx);

        auto deleter = [=](std::remove_extent_t<T>* p) noexcept {
            for (size_t i = 0; i < n; ++i)
                Tr::destroy(alloc, p + i);
            Tr::deallocate(alloc, p, n);
        };

        return {ptr, std::move(deleter), alloc};
    }
    catch (...)
    {
        for (; idx > 0; --idx)
            Tr::destroy(alloc, ptr + (idx - 1));
        Tr::deallocate(alloc, ptr, n);
        throw;
    }
}

template <class T, bool ThreadSafe = false, class... Args, std::enable_if_t<!std::is_array_v<T>, int> = 0>
SharedRef<T, ThreadSafe> MakeShared(Args&&... args)
{
    return AllocateShared<T, ThreadSafe>(std::allocator<T>{}, std::forward<Args>(args)...);
}

template <class T, bool ThreadSafe = false, std::enable_if_t<std::is_array_v<T>, int> = 0>
SharedRef<T, ThreadSafe> MakeShared(size_t n)
{
    return AllocateShared<T, ThreadSafe>(std::allocator<std::remove_extent_t<T>>{}, n);
}

template <class To, class From, bool ThreadSafe>
SharedPtr<To, ThreadSafe> StaticCast(SharedPtr<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), static_cast<To*>(ptr.get())};
}

template <class To, class From, bool ThreadSafe>
SharedRef<To, ThreadSafe> StaticCast(SharedRef<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), static_cast<To*>(ptr.get())};
}

template <class To, class From, bool ThreadSafe>
SharedPtr<To, ThreadSafe> ConstCast(SharedPtr<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), const_cast<To*>(ptr.get())};
}

template <class To, class From, bool ThreadSafe>
SharedRef<To, ThreadSafe> ConstCast(SharedRef<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), const_cast<To*>(ptr.get())};
}

template <class To, class From, bool ThreadSafe>
SharedPtr<To, ThreadSafe> ReinterpretCast(SharedPtr<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), reinterpret_cast<To*>(ptr.get())};
}

template <class To, class From, bool ThreadSafe>
SharedRef<To, ThreadSafe> ReinterpretCast(SharedRef<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), reinterpret_cast<To*>(ptr.get())};
}

/**
 * @brief Casts the pointer dynamically.
 * @tparam To Target type
 * @tparam From Source type
 * @param ptr The pointer
 * @return Casted pointer if successful, null otherwise.
 */
template <class To, class From, bool ThreadSafe>
SharedPtr<To, ThreadSafe> Cast(SharedPtr<From, ThreadSafe> ptr) noexcept
{
    return {std::move(ptr), dynamic_cast<To*>(ptr.get())};
}

/**
 * @brief Casts the pointer dynamically.
 * @tparam To Target type
 * @tparam From Source type
 * @param ptr The pointer
 * @return Casted pointer
 * @throw std::bad_cast If failed to cast.
 */
template <class To, class From, bool ThreadSafe> SharedRef<To, ThreadSafe> Cast(SharedRef<From, ThreadSafe> ptr)
{
    return {std::move(ptr), &dynamic_cast<To&>(*ptr)};
}

/**
 * Casts the object pointer statically and asserts that it is safe to cast.
 * If NDEBUG is not defined, uses dynamic_cast to verify that it is safe to cast.
 * @param ptr Pointer
 * @return Casted pointer
 */
template <class To, class From, bool ThreadSafe>
[[nodiscard]] SharedPtr<To, ThreadSafe> CastChecked(SharedPtr<From, ThreadSafe> ptr) noexcept
{
    if (!ptr)
        return {};

    assert(dynamic_cast<To*>(ptr.get()));
    return StaticCast<To>(std::move(ptr));
}

/**
 * Casts the object pointer statically and asserts that it is safe to cast.
 * If NDEBUG is not defined, uses dynamic_cast to verify that it is safe to cast.
 * @param ptr Pointer
 * @return Casted pointer.
 */
template <class To, class From, bool ThreadSafe>
[[nodiscard]] SharedRef<To, ThreadSafe> CastChecked(SharedRef<From, ThreadSafe> ptr) noexcept
{
    assert(dynamic_cast<To*>(ptr.get()));
    return StaticCast<To>(std::move(ptr));
}

template <class T, bool ThreadSafe>
constexpr bool operator==(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p == nullptr;
}

template <class T, bool ThreadSafe>
constexpr bool operator!=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p != nullptr;
}

template <class T, bool ThreadSafe> constexpr bool operator<(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p < nullptr;
}

template <class T, bool ThreadSafe> constexpr bool operator>(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p > nullptr;
}

template <class T, bool ThreadSafe>
constexpr bool operator<=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p <= nullptr;
}

template <class T, bool ThreadSafe>
constexpr bool operator>=(std::nullptr_t, const SharedPtr<T, ThreadSafe>& p) noexcept
{
    return p >= nullptr;
}

template <class T, bool ThreadSafe> void swap(SharedPtr<T, ThreadSafe>& l, SharedPtr<T, ThreadSafe>& r) noexcept
{
    l.swap(r);
}

template <class T, bool ThreadSafe> void swap(WeakPtr<T, ThreadSafe>& l, WeakPtr<T, ThreadSafe>& r) noexcept
{
    l.swap(r);
}

template <class T, bool ThreadSafe> SharedRef(SharedPtr<T, ThreadSafe>) -> SharedRef<T, ThreadSafe>;

} // namespace core
} // namespace oeng

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <class T, bool ThreadSafe> struct std::hash<oeng::core::SharedPtr<T, ThreadSafe>>
{
    size_t operator()(const oeng::core::SharedPtr<T, ThreadSafe>& p) const noexcept
    {
        return std::hash<T*>{}(p.get());
    }
};
#endif
