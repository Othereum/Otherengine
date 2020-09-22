#pragma once
#include "Path.hpp"

namespace oeng
{
inline namespace core
{

class CORE_API Object : public EnableSharedFromThis<Object>
{
  public:
    virtual ~Object() = default;

    [[nodiscard]] virtual Name GetClassName() const noexcept = 0;

    void from_json(const Json& json, Path path)
    {
        path_ = path;
        from_json(json);
    }

    [[nodiscard]] Path GetPath() const noexcept
    {
        return path_;
    }

    template <class T>[[nodiscard]] bool IsA() const noexcept
    {
        return dynamic_cast<T*>(this);
    }

  protected:
    virtual void from_json([[maybe_unused]] const Json& json)
    {
    }

  private:
    Path path_;
};

/**
 * Creates an object with a class name.
 * @param type Class name.
 * @return Created object.
 * @throw std::out_of_range If the type name is not valid.
 */
[[nodiscard]] CORE_API SharedRef<Object> NewObject(Name type);

/**
 * Creates an object with a class name and casts it to the target type.
 * @tparam T Target type.
 * @param type Class name.
 * @return Created object.
 * @throw std::out_of_range If the type name is not valid.
 * @throw std::bad_cast If the cast fails.
 */
template <class T>[[nodiscard]] SharedRef<T> NewObject(Name type)
{
    return Cast<T>(NewObject(type));
}

/**
 * Used by CLASS_BODY() macro. DO NOT USE IT MANUALLY.
 */
CORE_API void RegisterClass(Name type, SharedRef<Object> (*creator)());

} // namespace core
} // namespace oeng
