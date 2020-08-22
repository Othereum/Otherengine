#pragma once
#include "CompPair.hpp"

namespace oeng::core
{
/**
 * RAII resource handler
 */
template <std::regular T, std::invocable<T> Deleter>
class Resource
{
public:
    Resource() = default;

    explicit Resource(T id)
        : storage_{ZeroThen{}, std::move(id)}
    {
    }

    Resource(T id, Deleter deleter)
        : storage_{OneThen{}, std::move(deleter), std::move(id)}
    {
    }

    ~Resource()
    {
        storage_.first()(storage_.second());
    }

    Resource(Resource&& r) noexcept
        : storage_{std::move(r)}
    {
        r = {};
    }

    Resource& operator=(Resource&& r) noexcept
    {
        Resource{std::move(r)}.swap(*this);
        return *this;
    }

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    void reset(T id)
    {
        Resource{std::move(id)}.swap(*this);
    }

    void reset(T id, Deleter deleter)
    {
        Resource{std::move(id), std::move(deleter)}.swap(*this);
    }

    T& operator*() noexcept
    {
        return storage_.second();
    }

    const T& operator*() const noexcept
    {
        return storage_.second();
    }

    T* operator->() noexcept
    {
        return &storage_.second();
    }

    const T* operator->() const noexcept
    {
        return &storage_.second();
    }

    void swap(Resource& r) noexcept
    {
        using std::swap;
        swap(storage_, r.storage_);
    }

private:
    CompPair<Deleter, T> storage_;
};

template <std::regular T, std::invocable<T> Deleter>
void swap(Resource<T, Deleter>& a, Resource<T, Deleter>& b) noexcept
{
    a.swap(b);
}
}
