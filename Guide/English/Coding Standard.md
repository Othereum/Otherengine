# Coding Standard

[한국어](../한국어/코딩%20표준.md)

## C-style string API

The C-style API uses a null-terminated string. However, sometimes a length including null characters is used, and sometimes an excluded length is used. This can cause confusion, so you should clearly distinguish by name.

* Length including null characters should be called **buffer size**.
* Length excluding null characters should be called **string length**.

All APIs in `std::basic_string` use length excluding null characters.

## null safety

A function that returns a pointer should not normally return `nullptr`. Instead, return a null object or throw an exception.
