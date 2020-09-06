#pragma once
#include <string_view>

#ifdef _WIN32
#define OE_EXPORT __declspec(dllexport)
#define OE_IMPORT __declspec(dllimport)
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define OE_IMPORT __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#define OE_IMPORT __global
#else
#define OE_IMPORT
#endif
#define OE_EXPORT OE_IMPORT
#endif

#define EXPLICIT_CPMV(name, spec)                                                                                      \
    name(const name&) = spec;                                                                                          \
    name(name&&) = spec;                                                                                               \
    name& operator=(const name&) = spec;                                                                               \
    name& operator=(name&&) = spec

#define DELETE_CPMV(name) EXPLICIT_CPMV(name, delete)
#define DEFAULT_CPMV(name) EXPLICIT_CPMV(name, default)

/**
 * Helper macro that must be used for classes derived from `Object`.
 *
 * Example:
 * @code{cpp}
 * class AMyActor : public AActor
 * {
 * CLASS_BODY(AMyActor)
 *
 * public:
 *     // ...
 * }
 * @endcode
 */
#define CLASS_BODY(name)                                                                                               \
  public:                                                                                                              \
    inline static const Name class_name = u8## #name##sv;                                                              \
                                                                                                                       \
    [[nodiscard]] static std::shared_ptr<Object> Create()                                                              \
    {                                                                                                                  \
        return std::make_shared<name>();                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    [[nodiscard]] Name GetClassName() const noexcept override                                                          \
    {                                                                                                                  \
        return class_name;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    [[nodiscard]] std::shared_ptr<name> shared_from_this()                                                             \
    {                                                                                                                  \
        return std::static_pointer_cast<name>(Object::shared_from_this());                                             \
    }                                                                                                                  \
                                                                                                                       \
    [[nodiscard]] std::shared_ptr<const name> shared_from_this() const                                                 \
    {                                                                                                                  \
        return std::static_pointer_cast<const name>(Object::shared_from_this());                                       \
    }                                                                                                                  \
                                                                                                                       \
    [[nodiscard]] std::weak_ptr<name> weak_from_this()                                                                 \
    {                                                                                                                  \
        return std::static_pointer_cast<name>(Object::shared_from_this());                                             \
    }                                                                                                                  \
                                                                                                                       \
    [[nodiscard]] std::weak_ptr<const name> weak_from_this() const                                                     \
    {                                                                                                                  \
        return std::static_pointer_cast<const name>(Object::shared_from_this());                                       \
    }                                                                                                                  \
                                                                                                                       \
  private:                                                                                                             \
    class _##name##Registerer                                                                                          \
    {                                                                                                                  \
      public:                                                                                                          \
        _##name##Registerer()                                                                                          \
        {                                                                                                              \
            RegisterClass(class_name, &Create);                                                                        \
        }                                                                                                              \
    };                                                                                                                 \
                                                                                                                       \
    inline static const _##name##Registerer _##name##_registerer_;

/**
 * Helper macro for interface classes.
 *
 * It declares default constructor, defaulted virtual destructor,
 * and deletes copy/move constructor/assignment operators.
 *
 * Example:
 * @code{cpp}
 * class IMyInterface
 * {
 * INTERFACE_BODY(IMyInterface)
 *
 * public:
 *     // ...
 * }
 * @endcode
 */
#define INTERFACE_BODY(name)                                                                                           \
  public:                                                                                                              \
    name() = default;                                                                                                  \
    virtual ~name() = default;                                                                                         \
    DELETE_CPMV(name);                                                                                                 \
  private:

#define U8_TEXT_IMPL(x) u8##x
#define U8_TEXT(x) U8_TEXT_IMPL(x)
#define U8_FILE U8_TEXT(__FILE__)

#define STRINGIZE_IMPL(x) #x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define U8_LINE U8_TEXT(STRINGIZE(__LINE__))

namespace oeng
{
inline namespace core
{
[[nodiscard]] CORE_API std::u8string_view GetGameName() noexcept;

/**
 * Check if the current thread is a game thread.
 * @return True if called from the game thread.
 */
[[nodiscard]] CORE_API bool IsGameThread() noexcept;
}
}
