#pragma once
#include "Serializable.hpp"

namespace oeng::core
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

    virtual void Serialize(void* data, std::streamsize size) = 0;

    Archive& operator<<(ISerializable& obj)
    {
        obj.Serialize(*this);
        return *this;
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
};
}
