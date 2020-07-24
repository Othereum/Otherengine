# Asserts

[한국어](../한국어/어서트.md)

Assert is a tool that verifies the situation assumed by a given piece of code. A typical example is to validate a pointer for NULL. Otherengine provides a series of macros for this type of validation. Under certain build configurations, macros are made to exclude them from compilation, either for performance reasons or not needed in the final build.

Every assertion macro triggers a breakpoint if the expression is not true and if it is running inside the debugger, so you can investigate how it got there.

---

## assert(expression)

This macro evaluates the expression, and then stops running if the assertion result is false. The expression is evaluated only if the `NDEBUG` macro is defined. It is part of a C standard library.

```cpp
#include <cassert>

assert(mesh != nullptr);
assert(was_initialized && "Did you forget to call Init()?");
```

---

The macros below do not stop execution, but are used to log errors that help track problems. `Debug.hpp` should be included.

## ENSURE(expression)

The expression of this macro is always evaluated and can be placed within a conditional expression. If `NDEBUG` is defined, logs the error message including the full file path of the source code and line number.

Example:

```cpp
if (ENSURE(in_object != nullptr))
{
    InObject->Modify();
}
```

## ENSURE_MSG(expression, message, ...)

It is the same as `ENSURE()`, but the error message may contain information to help track the problem.

Example:

```cpp
if (ENSURE_MSG(!modal, u8"Could not create dialog because modal is set to {}"sv, modal))
{
    // ...
}
```

## EXPECT(expression)

This macro is similar to `ENSURE()`, but does not evaluate expression if `NDEBUG` is defined. Cannot be placed within a conditional expression.

Example:

```cpp
EXPECT(list.HasCycle());
```

## EXPECT_MSG(expression, message, ...)

It is the same as `EXPECT()`, but the error message may contain information to help track the problem.

Example:

```cpp
EXPECT_MSG(!modal, u8"Could not create dialog because modal is set to {}"sv, modal)
```

## EXPECT_NO_ENTRY()

This macro does not accept expressions. Used to indicate a code path that will never executed.

Example:

```cpp
switch (my_enum)
{
    case MyEnum::value1:
        break;
    default:
        EXPECT_NO_ENTRY();
        break;
}
```

## SHOULD(expression, message, ...)

This macro is always active, whether or not `NDEBUG` is defined, and can be placed in a conditional expression, as with `ENSURE()`. However, the error message does not include the file path and line number.

Example:

```cpp
if (SHOULD(!modal, u8"Could not create dialog because modal is set to {}"sv, modal))
{
    // ...
}
```
