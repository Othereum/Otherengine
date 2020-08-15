#pragma once
#include "Archive.hpp"
#include <fstream>

namespace oeng::core
{
class ArchiveFileReader final : public Archive
{
public:
    [[nodiscard]] bool IsLoading() const noexcept override
    {
        return true;
    }

private:
    std::ifstream stream_;
};
}
