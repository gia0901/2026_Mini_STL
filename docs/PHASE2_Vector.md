# Phase 2: Vector - Detailed Implementation Guide

## 1. Introduction
`vector` is a dynamic array and the most important, frequently used container in C++. It provides $O(1)$ random access and amortized $O(1)$ insertion at the end. 
This guide provides explicit, detailed instructions for implementing `gstd::vector` to ensure you don't get stuck on memory management hurdles. 

### Deliverables
* `gstd::vector` class template
* Iterators for `vector` (raw pointers are sufficient for our MVP)
* Comprehensive memory management and tests

---

## 2. Essential Theory: Memory Management (The Hard Part)

The biggest mistake beginners make when writing a vector is using `new T[capacity]`.
**Why is `new T[capacity]` wrong?**
1. It forces `T` to have a default constructor.
2. It immediately constructs `capacity` number of objects, even if `size` is 0. This is a huge performance penalty and semantically incorrect.

Instead, a proper vector separates **memory allocation** from **object construction**.

### 1. Memory Allocation (`::operator new`)
To allocate raw, uninitialized bytes without calling constructors, use:
```cpp
// Allocates enough bytes for 'capacity' number of T objects
T* data = static_cast<T*>(::operator new(capacity * sizeof(T)));
```

### 2. Object Construction (Placement `new`)
To construct an object into that raw, uninitialized memory, use placement `new`. This calls the constructor of `T` at the specific memory address you provide.
```cpp
// Constructs a copy of 'val' at the memory address &data[size]
new (&data[size]) T(val); 
```

### 3. Object Destruction
When removing an element (e.g., in `pop_back`), you must explicitly call its destructor because you used placement `new`.
```cpp
// Explicitly destroy the object at index (size - 1)
data[size - 1].~T();
```

### 4. Memory Deallocation (`::operator delete`)
When the vector is destroyed or reallocated, you free the raw memory buffer. Note that `delete[] data;` is wrong because it would try to call destructors on uninitialized memory.
```cpp
::operator delete(data);
```

---

## 3. Class Structure & Core Members

Start your `include/gstd/vector.hpp` with this exact structure to make implementation easier.

```cpp
namespace gstd {

template <typename T>
class vector {
public:
    // Typedefs (Standard STL practice)
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;         // Raw pointers work perfectly as random-access iterators!
    using const_iterator = const T*;

private:
    T* data_;           // Pointer to the raw memory buffer
    size_type size_;    // Number of active, constructed elements
    size_type capacity_;// Total number of elements the buffer can hold

    // --- Private Helper: The core reallocation logic ---
    void reallocate(size_type new_capacity);

public:
    // ... Constructors, Destructors, Methods ...
};

} // namespace gstd
```

---

## 4. Step-by-Step Implementation Guide

### Step 4.1: The Secret Sauce - `reallocate()`
Implement this private helper first. It handles capacity growth.
**Task:**
1. Check if `new_capacity > capacity_`. If not, do nothing.
2. Allocate a new raw memory buffer of size `new_capacity` using `::operator new`.
3. Loop from `0` to `size_ - 1`:
   - Move construct elements from the old `data_` to the new buffer using placement `new` and `gstd::move(data_[i])`.
   - Explicitly call the destructor on the old element: `data_[i].~T()`.
4. Deallocate the old memory buffer using `::operator delete(data_)`.
5. Update `data_ = new_buffer` and `capacity_ = new_capacity`.

### Step 4.2: Constructors & Destructor
**Task:**
1. **Default Constructor:** Initialize `data_(nullptr)`, `size_(0)`, `capacity_(0)`.
2. **Fill Constructor `vector(size_type count, const T& value)`:**
   - Set `size_` and `capacity_` to `count`.
   - Allocate raw memory via `::operator new`.
   - Loop `count` times, using placement `new` to copy-construct `value` into the buffer.
3. **Copy Constructor `vector(const vector& other)`:**
   - Set `size_` and `capacity_` to match `other`.
   - Allocate memory.
   - Loop and use placement `new` to construct a copy of each element from `other`.
4. **Move Constructor `vector(vector&& other)`:**
   - Steal pointers: `data_ = other.data_`, `size_ = other.size_`, `capacity_ = other.capacity_`.
   - **Crucial:** Nullify `other` (`other.data_ = nullptr`, `other.size_ = 0`, `other.capacity_ = 0`) so its destructor doesn't free your newly stolen data.
5. **Destructor `~vector()`:**
   - Call `clear()` to destroy all active objects.
   - Deallocate the memory buffer with `::operator delete(data_)`.

### Step 4.3: Modifiers (`push_back`, `pop_back`, etc.)
**Task:**
1. **`push_back(const T& value)` / `push_back(T&& value)`:**
   - If `size_ == capacity_`, call `reallocate(capacity_ == 0 ? 1 : capacity_ * 2)`.
   - Use placement `new` to construct the new value at `&data_[size_]`.
   - Increment `size_`.
2. **`pop_back()`:**
   - Add a debug assertion: `assert(size_ > 0);`.
   - Call the destructor on the last element: `data_[size_ - 1].~T()`.
   - Decrement `size_`. *(Do not free memory or shrink capacity)*.
3. **`clear()`:**
   - Loop and call `.~T()` on all `size_` elements.
   - Set `size_ = 0`. *(Do not change capacity or deallocate)*.
4. **`reserve(size_type new_cap)`:**
   - If `new_cap > capacity_`, call `reallocate(new_cap)`.

### Step 4.4: Element Access & Capacity
**Task:**
1. **`size()` / `capacity()`:** Return their respective variables.
2. **`empty()`:** Return `size_ == 0`.
3. **`operator[](size_type pos)`:**
   - Add a debug assertion: `assert(pos < size_);`.
   - Return `data_[pos]`.
4. **`front()` / `back()`:**
   - Return `data_[0]` and `data_[size_ - 1]`. Assert vector is not empty.

### Step 4.5: Iterators
**Task:**
1. **`begin()`:** Return `data_`.
2. **`end()`:** Return `data_ + size_`.
(Because memory is contiguous, `data_ + size_` naturally points to one-past-the-last active element).

---

## 5. Testing Strategy

Set up GoogleTest in `tests/unit_tests/test_vector.cpp`.

1. **The "No Default Constructor" Test:**
   Create a struct with `MyStruct() = delete;` and only a parameterized constructor. Attempt to create a `gstd::vector<MyStruct>` and `reserve(10)`. If you used `new T[]`, this will fail to compile. It *must* compile and run cleanly.
2. **Memory Leak & Destructor Test:**
   Create a `Tracker` class that increments a global static variable in its constructor, and decrements it in its destructor. 
   - `push_back` 10 elements.
   - Call `clear()`. Assert the global counter is 0.
3. **Growth Test:**
   Loop `push_back` 1000 integers. Check that `size()` is 1000 and `capacity()` $\ge$ 1000, confirming your doubling logic works seamlessly.
4. **Move Semantics Test:**
   Ensure `gstd::vector` handles move-only types like `std::unique_ptr` correctly in `push_back` and `reallocate`.

---

## 6. Interview Preparation (Self-Review)

Be prepared to answer these questions confidently:

* **Q: Why does `push_back` have an *amortized* $O(1)$ time complexity?**
  * **A:** Most of the time, `push_back` just does a placement new and increments a counter, which is $O(1)$. When capacity is full, it must allocate new memory, copy/move all $N$ elements, which takes $O(N)$. However, because we double the capacity each time, this $O(N)$ work happens increasingly rarely. If you average the cost over $N$ insertions, the cost scales down to a constant $O(1)$ per insertion.
* **Q: What is "Iterator Invalidation" and when does it happen in a vector?**
  * **A:** Iterators in a vector are essentially pointers to elements in the underlying array. If an operation (like `push_back` or `reserve`) triggers a reallocation, the old array is destroyed and moved to a new memory address. All existing iterators, pointers, and references pointing to the old array become dangling (invalid).
* **Q: Why do we use placement `new` instead of normal `new` or `malloc`?**
  * **A:** `malloc` provides raw memory but doesn't call C++ constructors, meaning objects won't be initialized properly. Normal `new` both allocates memory *and* constructs the object immediately (requiring a default constructor). Placement `new` gives us the best of both worlds: it constructs an object at a specific memory location that we have already pre-allocated, allowing us to perfectly separate allocation from initialization.
