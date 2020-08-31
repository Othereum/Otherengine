#pragma once
#include "Archive.hpp"
#include "File.hpp"

namespace oeng
{
inline namespace core
{
class CORE_API ArchiveFileReader final : public Archive
{
public:
    /**
     * Open the file in read mode.
     * @param filepath File path.
     * @throw std::ios::failure If failed to open file.
     */
    explicit ArchiveFileReader(const fs::path& filepath)
        : Archive{filepath.u8string()},
          stream_{ReadFile(filepath, std::ios::binary | std::ios::ate)},
          size_{SafeCast<size_t>(static_cast<std::streamoff>(stream_.tellg()))}
    {
        stream_.seekg(0, std::ios::beg);
    }

    void Serialize(void* bytes, size_t num_bytes) override
    {
        stream_.read(static_cast<char8_t*>(bytes), SafeCast<std::streamsize>(num_bytes));
    }

    void Seek(size_t pos) override
    {
        stream_.seekg(SafeCast<std::streamoff>(pos));
    }

    void Seek(intptr_t off, std::ios::seekdir dir) override
    {
        stream_.seekg(SafeCast<std::streamoff>(off), dir);
    }

    [[nodiscard]] size_t Tell() override
    {
        return stream_.tellg();
    }

    [[nodiscard]] size_t Size() override
    {
        return size_;
    }

    [[nodiscard]] explicit operator bool() const noexcept override
    {
        return stream_.operator bool();
    }

    [[nodiscard]] bool IsLoading() const noexcept override
    {
        return true;
    }

private:
    std::basic_ifstream<char8_t> stream_;
    size_t size_;
};
}
}
