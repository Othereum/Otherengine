# Memory

[한국어](../한국어/메모리.md)

## Memory Pool

Otherengine uses memory pool. There are several advantages to using it.

1. Prevent memory fragmentation. This is especially helpful for mobile devices or consoles with less memory.
2. High performance in repetitive memory allocation/release.
3. Cache friendly.

But there are limitations also.

1. Not available in multi-threaded environments. This is because syncing can significantly reduce performance. So it's limited to game threads only. Of course, game modules rarely use multi-threads, so you don't have to worry about it in general.
2. Static objects and their own objects cannot use memory pools. The memory pool is owned by an engine object, which does not exist at the time of creation of a static object. The same goes for destruction.

In the above case, do not use memory pools, but use the C++ standard memory allocation/release mechanism.

This is how you can use the memory pool:

```cpp
#include "Memory.hpp"

class MyClass {...};
class MyDerived : public MyClass {...};

auto ptr = New<int>(4); // Initialize *ptr to 4
Delete(ptr); // Delete<int>(ptr)

auto ptr2 = New<MyClass>(...); // Forward ... to the constructor of MyClass
Delete(ptr2); // Delete<MyClass>(ptr2)

auto ptr_arr = NewArr<int>(5, ...) // int[5]; Initialize with ...
DeleteArr(ptr_arr, 5) // IMPORTANT: You must pass the same size

MyClass* ptr_upcast = New<MyDerived>(...);
Delete(ptr_upcast) // Delete<MyClass>(ptr_upcast)
// ^^^^^^^^^
// YOU SHOULD NEVER DO THIS
// The reason is explained in detail below...
```

The size of memory pool is fixed. So you have to select the block set that matches the size of memory you need. For example, if you need 612 bytes of memory, it allocated from a memory pool that has a 1024-byte block set. Therefore, you must return it to a memory pool of the same size when you delete it.

`New<T>()` and `Delete<T>()` figures out the size by `sizeof(T)`. This means that the object size is statically deduced. Therefore, it is assumed as the size of the pointed type, not the actual size of the object. Thus, it will be **returned to a different memory pool** than it was allocated to. **Debugging is extremely difficult** because the error does not occur immediately.

There is no problem with the [`SharedPtr`] described below. This is because the deleter is determined at the time the [`SharedPtr`] is created.

## [`SharedPtr`]

Otherengine's dynamic object management takes place through smart pointers. Among them, `shared_ptr` does not use a standard library, but uses a custom [`SharedPtr`]. The reasons are as follows:

1. Memory Pool - Object creation and deletion must be done in the memory pool.
2. Unnecessary thread synchronization - Objects are rarely shared by multiple threads.

To create a new shared object, use `MakeShared` instead of `std::make_shared`:

```cpp
#include "Templates/Pointer.hpp"

auto obj = MakeShared<T>(...);

// Use custom allocator
MyAlloc<T> alloc;
auto alobj = AllocateShared<T>(alloc, ...);
```

The return type of `MakeShared` is not [`SharedPtr`] but [`SharedRef`]. [`SharedRef`] is a non-nullable [`SharedPtr`]. Instead of [`SharedPtr`] throughout the code, [`SharedRef`] can be used to eliminate null checks and write safer, easier-to-read codes.

> **WARNING**: By default, when objects are deleted, they go through memory pools. Therefore, if you create a reference target directly from the raw pointer or change the reference target through `reset(p)`, you must use only the objects allocated from the memory pool or specify the appropriate deleter directly.

[`SharedPtr`]: https://othereum.github.io/Otherengine/classoeng_1_1core_1_1_shared_ptr.html
[`SharedRef`]: https://othereum.github.io/Otherengine/classoeng_1_1core_1_1_shared_ref.html
