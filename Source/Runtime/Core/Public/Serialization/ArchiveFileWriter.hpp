#pragma once
#include "Archive.hpp"
#include "File.hpp"

namespace oeng
{
inline namespace core
{
class CORE_API ArchiveFileWriter final : public Archive
{
public:
    /**
     * Open the file in write mode.
     * If there are no directories corresponding to the specified path, they are automatically created.
     * @param filepath File path.
     * @throw std::ofstream::failure If failed to open file.
     * @throw std::filesystem::filesystem_error If the path is invalid.
     */
    explicit ArchiveFileWriter(const fs::path& filepath)
        : Archive{filepath.u8string()},
          stream_{WriteFile(filepath, std::ofstream::binary)}
    {
    }

    void Serialize(void* data, size_t size) override
    {
        stream_.write(static_cast<const char8_t*>(data), SafeCast<std::streamsize>(size));
    }

    void Seek(size_t pos) override
    {
        stream_.seekp(SafeCast<std::streamoff>(pos));
    }

    void Seek(intptr_t off, std::ios::seekdir dir) override
    {
        stream_.seekp(SafeCast<std::streamoff>(off), dir);
    }

    [[nodiscard]] size_t Tell() override
    {
        return SafeCast<size_t>(static_cast<std::streamoff>(stream_.tellp()));
    }

    [[nodiscard]] explicit operator bool() const noexcept override
    {
        return stream_.operator bool();
    }

    [[nodiscard]] bool IsLoading() const noexcept override
    {
        return false;
    }

private:
    std::basic_ofstream<char8_t> stream_;
};
}
