#pragma once
#include "Archive.hpp"
#include "File.hpp"

namespace oeng::core
{
class CORE_API ArchiveFileReader : public Archive
{
public:
    /**
     * Open the file in read mode.
     * @param filepath File path.
     * @throw std::ios::failure If failed to open file.
     */
    explicit ArchiveFileReader(const fs::path& filepath)
        : stream_{ReadFile(filepath, std::ios::binary | std::ios::ate)},
          size_{stream_.tellg()}
    {
        stream_.seekg(0, std::ios::beg);
    }

    void Serialize(void* data, std::streamsize size) override
    {
        stream_.read(static_cast<char*>(data), size);
    }

    [[nodiscard]] explicit operator bool() const noexcept override
    {
        return stream_.operator bool();
    }

    [[nodiscard]] bool IsLoading() const noexcept override
    {
        return true;
    }

    void Seek(std::streampos pos) override
    {
        stream_.seekg(pos);
    }

    void Seek(std::streamoff off, std::ios::seekdir dir) override
    {
        stream_.seekg(off, dir);
    }

    [[nodiscard]] std::streampos Tell() override
    {
        return stream_.tellg();
    }

    [[nodiscard]] std::streamsize Size() override
    {
        return size_;
    }

private:
    std::ifstream stream_;
    std::streamsize size_;
};
}
