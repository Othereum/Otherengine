#pragma once

namespace oeng::core
{
class Archive
{
public:
    INTERFACE_BODY(Archive);

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
};
}
