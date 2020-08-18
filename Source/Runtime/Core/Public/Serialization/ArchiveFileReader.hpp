#pragma once
#include "Archive.hpp"
#include <fstream>

namespace oeng::core
{
class ArchiveFileReader final : public Archive
{
public:
    explicit ArchiveFileReader(const fs::path& filepath)
        : stream_{filepath, std::ios_base::in | std::ios_base::binary}
    {
    }

    [[nodiscard]] bool IsLoading() const noexcept override
    {
        return true;
    }

    Archive& operator<<(Json& json) override;

private:
    std::ifstream stream_;
};
}
