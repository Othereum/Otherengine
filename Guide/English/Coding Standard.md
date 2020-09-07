# Coding Standard

[한국어](../한국어/코딩%20표준.md)

## C++

### C++20

Otherengine uses C++20.

## String

### `char8_t`

`char8_t`is a new type added since C++20 to represent UTF-8 characters. Otherengine uses `char8_t` as the default character type. Use `char8_t` literal as follows:

```cpp
// char8_t
auto c = u8'a';

// char8_t*
auto str = u8"Test string";
```

### `string_view`

Use `std::u8string_view` rather than C style null-terminated string `char8_t*` for non-owned strings. Use the `u8string_view` literal as follows:

```cpp
// std::u8string_view
auto str = u8"Test string"sv;
```

### C-style string API

The C-style API uses a null-terminated string. However, sometimes a length including null characters is used, and sometimes an excluded length is used. This can cause confusion, so you should clearly distinguish by name.

* Length including null characters should be called **buffer size**.
* Length excluding null characters should be called **string length**.

NOTE: All APIs in `std::basic_string` use length excluding null characters.

## Object

### Null Safety

It is not recommended that the function that returns the pointer returns `nullptr`. This is because the more null checks in the code, the less readable. Consider returning a null object or throwing an exception instead.

### Object Management

Otherengine uses smart pointers instead of GC to manage objects. So the engine user also uses a smart pointer in the code, and there are a few precautions.

1. All classes derived from the `Object` must be managed by `shared_ptr`.

1. When referencing actors/components, `shared_ptr` should not be used except in special cases. Because the component is owned by the actor, if it is referenced elsewhere by `shared_ptr`, the component will not be destroyed even if the owner actor is destroyed. Similarly, because the actor is owned by the `World`, even if you call `Destroy()` explicitly, it will only be removed from the World's Actors list, so if you have another shared_ptr, the actor will still be alive. In both cases, `weak_ptr` can be used to resolve the problem. The raw pointer is also a good choice if you are sure that the referenced target is **not destroyed** before referencer.

To avoid problems, you must first clarify the ownership/reference relationship between objects before writing code.

### Class Definition

When defining `Object` derived classes, the macro `CLASS_BODY(ClassName)` must be placed at the top. For example:

```cpp
class AMyPlayerController : public APlayerController
{
CLASS_BODY(AMyPlayerController)

public:
    // ...
```

The main role of this macro is to allow you to create an instance with the class name at run-time. Below is an example of creation (of course this is only an example, you should not create the actors manually):

```cpp
auto obj = NewObject(u8"AMyPlayerController"sv);

// NewObject + Cast
auto controller = NewObject<AController>(u8"AMyPlayerController"sv);
```
