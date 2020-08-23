#pragma once
#include "Serializable.hpp"

namespace oeng
{
inline namespace core
{
template <class T>
concept Trivial = std::is_trivial_v<T>;

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
    INTERFACE_BODY(Archive);

    explicit Archive(std::u8string name)
        : name_{std::move(name)}
    {
    }

    virtual void Serialize(void* data, std::streamsize size) = 0;

    Archive& operator<<(ISerializable& obj)
    {
        obj.Serialize(*this);
        return *this;
    }

    /**
     * Reads all remaining content in the buffer. Valid only for loading archives.
     * @tparam T Trivial type to store data.
     * @return first Read data.
     * @return second Number of elements read.
     * @throw WrongArchiveDirection If attempted to call on saving archive.
     */
    template <Trivial T>
    [[nodiscard]] std::pair<std::unique_ptr<T[]>, std::streamsize> ReadAll()
    {
        if (!IsLoading())
            throw WrongArchiveDirection{false};

        const auto count = (Size() - Tell()) / sizeof(T);
        if (count <= 0)
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

    virtual void Seek(std::streampos pos) = 0;
    virtual void Seek(std::streamoff off, std::ios::seekdir dir) = 0;

    [[nodiscard]] virtual std::streampos Tell() = 0;

    [[nodiscard]] virtual std::streamsize Size()
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
