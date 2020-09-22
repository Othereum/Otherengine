#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIMesh
{
  public:
    explicit RHIMesh(size_t num_indices) : num_indices_(num_indices)
    {
    }

    virtual ~RHIMesh() = default;
    virtual void Activate() const noexcept = 0;
    virtual void Draw() const noexcept = 0;

    [[nodiscard]] size_t GetNumIndices() const noexcept
    {
        return num_indices_;
    }

  private:
    const size_t num_indices_;
};
} // namespace rhi
} // namespace oeng
