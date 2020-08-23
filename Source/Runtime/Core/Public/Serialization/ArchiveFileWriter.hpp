#pragma once
#include "Archive.hpp"
#include "File.hpp"

namespace oeng::core
{
class CORE_API ArchiveFileWriter : public Archive
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

    void Serialize(void* data, std::streamsize size) override
    {
        stream_.write(static_cast<char*>(data), size);
    }

    void Seek(std::streampos pos) override
    {
        stream_.seekp(pos);
    }

    void Seek(std::streamoff off, std::ios::seekdir dir) override
    {
        stream_.seekp(off, dir);
    }

    [[nodiscard]] std::streampos Tell() override
    {
        return stream_.tellp();
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
    std::ofstream stream_;
};
}
