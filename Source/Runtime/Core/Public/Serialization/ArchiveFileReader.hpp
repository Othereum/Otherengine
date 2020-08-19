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
     * @throw std::ifstream::failure If failed to open file.
     */
    explicit ArchiveFileReader(const fs::path& filepath)
        : stream_{ReadFile(filepath, std::ifstream::binary)}
    {
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
    std::ifstream stream_;
};
}
