#pragma once
#include "Math.hpp"

namespace oeng
{
inline namespace core
{
template <class T>
concept POD = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

class WrongArchiveDirection : public std::exception
{
public:
    explicit WrongArchiveDirection(bool was_loading)
        : was_loading_{was_loading}
    {
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        return was_loading_
                   ? "Expected saving archive, but it was loading"
                   : "Expected loading archive, but it was saving";
    }

    [[nodiscard]] bool WasLoading() const noexcept
    {
        return was_loading_;
    }

private:
    bool was_loading_;
};

class Archive
{
public:
    explicit Archive(std::u8string name)
        : name_{std::move(name)}
    {
    }

    virtual void Serialize(void* bytes, size_t num_bytes) = 0;

    template <POD T>
    Archive& operator<<(T& data)
    {
        Serialize(&data, sizeof(T));
        return *this;
    }

    template <class T, class Tr, class Al>
    Archive& operator<<(std::basic_string<T, Tr, Al>& str)
    {
        auto len = SafeCast<uint32_t>(str.length());
        *this << len;

        str.resize(len);
        Serialize(str.data(), len * sizeof(T));

        return *this;
    }

    /**
     * Reads all remaining content in the buffer. Valid only for loading archives.
     * @tparam T Trivial type to store data.
     * @return first Read data.
     * @return second Number of elements read.
     * @throw WrongArchiveDirection If attempted to call on saving archive.
     */
    template <POD T>
    [[nodiscard]] std::pair<std::unique_ptr<T[]>, size_t> ReadAll()
    {
        if (!IsLoading())
            throw WrongArchiveDirection{false};

        const auto pos = Tell();
        const auto size = Size();
        const auto count = (size - pos) / sizeof(T);
        if (size <= pos || count == 0)
            return {};

        std::unique_ptr<T[]> data{new T[count]};
        Serialize(data.get(), count * sizeof(T));
        return {std::move(data), count};
    }

    [[nodiscard]] Json ReadAllAsJson()
    {
        const auto [raw, len] = ReadAll<char>();
        return Json::parse(raw.get(), raw.get() + len, nullptr, true, true);
    }

    virtual void Seek(size_t pos) = 0;
    virtual void Seek(intptr_t off, std::ios::seekdir dir) = 0;

    [[nodiscard]] virtual size_t Tell() = 0;

    [[nodiscard]] virtual size_t Size()
    {
        const auto pos = Tell();
        Seek(0, std::ios::end);
        const auto size = Tell();
        Seek(pos);
        return size;
    }

    /**
     * Checks whether the archive has no errors.
     * @return `true` if the archive has no errors, `false` otherwise.
     */
    [[nodiscard]] virtual explicit operator bool() const noexcept = 0;

    /**
     * Checks whether the archive is for loading data.
     * @return `true` if the archive is for loading data, `false` otherwise.
     */
    [[nodiscard]] virtual bool IsLoading() const noexcept = 0;

    [[nodiscard]] const std::u8string& GetName() const noexcept
    {
        return name_;
    }

protected:
    std::u8string name_;
};
}
}
