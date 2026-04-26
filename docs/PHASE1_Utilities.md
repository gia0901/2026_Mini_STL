# Phase 1: Utilities - Implementation Guide

## 1. Introduction
This guide covers Phase 1 of building the `gstd` library. Before diving into complex data structures like `vector` or `map`, we need to build foundational tools. These utilities are heavily used across all other STL components, particularly for memory optimization (move semantics) and generic programming (templates and traits).

### Deliverables
* `gstd::pair`
* `gstd::move`
* `gstd::forward`
* `gstd::swap`
* `gstd::iterator_traits`

*Note: All implementations should be strictly within the `gstd` namespace. Per the project philosophy, correctness, learning value, and mirroring `std::` APIs are the main priorities.*

---

## 2. Essential Theory

### Lvalues, Rvalues, and Rvalue References
To implement `move` and `forward`, you must fully grasp C++ value categories:
* **Lvalue (Locator Value):** An object that occupies some identifiable location in memory (has an address) and has a name. Example: `int x = 5;` (`x` is an lvalue).
* **Rvalue:** A temporary object that does not reside in a persistent memory location. It has no name. Example: The literal `5` or the result of `x + 2`.
* **Rvalue Reference (`&&`):** A reference that can only bind to rvalues. It allows us to distinguish between temporary objects and permanent ones, enabling *move semantics*. By moving from temporaries instead of copying, we avoid expensive allocations and deep copies.

### Type Traits and `remove_reference`
Before implementing `move`, you'll need a helper template to strip reference modifiers from a type. The C++ standard provides `std::remove_reference` for this.
If you have `T&`, it evaluates to `T`. If you have `T&&`, it evaluates to `T`. 
*Concept:* You use template specialization. Define a base template, then specialize it for `T&` and `T&&`.

### Reference Collapsing Rules
When working with templates (specifically with `T&&` which acts as a "Universal" or "Forwarding" reference in template deduction), you might end up with a reference to a reference. C++ collapses these according to specific rules:
* `T& &`   => `T&`
* `T& &&`  => `T&`
* `T&& &`  => `T&`
* `T&& &&` => `T&&`
*(Rule of thumb: If there's an lvalue reference anywhere, it collapses to an lvalue reference. Otherwise, it's an rvalue reference).* This is the core mechanism behind perfect forwarding.

---

## 3. Step-by-Step Implementation Guide

Create a file at `include/gstd/utility.hpp` (or spread across specific headers like `include/gstd/type_traits.hpp` if you prefer).

### Step 3.1: Helper - `remove_reference`
You must implement your own version of `std::remove_reference` to build `move` and `forward`.

**Task:**
1. Create a `struct remove_reference` taking a template parameter `T`. Provide a `using type = T;` inside.
2. Partially specialize it for `T&` (lvalue ref) and `T&&` (rvalue ref), both exposing `using type = T;`.
3. Provide an alias template: `template <typename T> using remove_reference_t = typename remove_reference<T>::type;` for cleaner usage.

### Step 3.2: `gstd::move`
**Theory:** `std::move` *does not move anything*. It unconditionally casts an object to an rvalue reference, telling the compiler, "You can steal my resources because I don't need them anymore."

**Task:**
1. Write a template function `move` that takes a forwarding reference `T&& arg`.
2. The return type should be `remove_reference_t<T>&&`.
3. Return a `static_cast` to that exact return type.

*Self-Check:* Because of the `remove_reference_t`, your `move` function guarantees that no matter what is passed in, an rvalue reference is returned.

### Step 3.3: `gstd::forward`
**Theory:** `std::forward` is used in template functions to pass an argument while preserving its original value category (lvalue remains lvalue, rvalue remains rvalue). This is called *perfect forwarding*.

**Task:**
1. Write two overloads for `forward`: one taking an lvalue reference (`remove_reference_t<T>&`), and one taking an rvalue reference (`remove_reference_t<T>&&`).
2. Both should return `T&&`.
3. Inside the functions, return `static_cast<T&&>(arg)`. The reference collapsing rules will do the heavy lifting for the return type.
4. *Safety check:* In the rvalue overload, consider using a `static_assert` to prevent forwarding an rvalue as an lvalue (checking that `T` is not an lvalue reference).

### Step 3.4: `gstd::swap`
**Theory:** A naive swap involves three copies (`temp = a`, `a = b`, `b = temp`). For large objects, this is incredibly expensive. We can optimize this by moving the resources instead.

**Task:**
1. Write a template function `swap(T& a, T& b)`.
2. Create a temporary `T temp = gstd::move(a);`.
3. Reassign `a = gstd::move(b);`.
4. Reassign `b = gstd::move(temp);`.

### Step 3.5: `gstd::pair`
**Theory:** A simple, heavily used struct holding two heterogeneous values.

**Task:**
1. Define a template `struct pair<T1, T2>`.
2. Provide public members `first` and `second`.
3. **Constructors:**
   - Default constructor.
   - Parameterized constructor (taking `const T1&` and `const T2&`).
   - Template constructor (taking `U1&&` and `U2&&`) to perfectly forward arguments into `first` and `second` using `gstd::forward`. (This avoids unnecessary copies when passing temporaries to a pair).
4. Implement copy/move constructors and assignment operators (often the compiler-generated ones are fine, but writing them out is good practice).

### Step 3.6: `gstd::iterator_traits`
**Theory:** Generic algorithms (like `gstd::find` or `std::distance`) need to know properties of the iterators they use (e.g., what type of element it points to, what its category is). For custom iterators, they have nested typedefs like `T::value_type`. However, plain pointers (`int*`) act as iterators for arrays, but pointers cannot have nested typedefs! `iterator_traits` provides a uniform interface to extract this info using template specialization.

**Task:**
1. Define empty marker structs for iterator categories if you haven't (e.g., `input_iterator_tag`, `forward_iterator_tag`, `random_access_iterator_tag`).
2. Define a base template `struct iterator_traits<Iterator>` that defines typedefs (`value_type`, `difference_type`, `pointer`, `reference`, `iterator_category`) by pulling them from the `Iterator` type (e.g., `using value_type = typename Iterator::value_type;`).
3. **Crucial Step:** Specialize `iterator_traits` for raw pointers `T*` (e.g., `value_type` is `T`, `iterator_category` is `random_access_iterator_tag`).
4. Specialize it for `const T*` (e.g., `value_type` is `T`, not `const T`).

---

## 4. Testing Strategy

Set up your tests in the `tests/` directory using GoogleTest.

1. **Test `move` and `swap`:**
   - Create a dummy "tracker" class that increments global counters whenever its copy constructor, copy assignment, move constructor, or move assignment is called.
   - Use `gstd::swap` on two instances. Assert that *only* move operations were called (counters for copy are 0).
2. **Test `forward`:**
   - Write a wrapper template function that forwards arguments to overloaded target functions (one taking lvalue, one taking rvalue).
   - Verify that passing an lvalue triggers the lvalue target, and passing an rvalue triggers the rvalue target.
3. **Test `pair`:**
   - Test default construction and value construction.
   - **Important:** Test moving a non-copyable object (like a mock `unique_ptr` or an explicitly non-copyable struct) into a `gstd::pair` to ensure your perfect forwarding template constructor works.
4. **Test `iterator_traits`:**
   - Use `static_assert` to check types at compile time. 
   - `static_assert(std::is_same_v<typename gstd::iterator_traits<int*>::value_type, int>, "Pointer trait failed");`

---

## 5. Interview Preparation (Self-Review)

Be prepared to answer these questions confidently out loud:

*   **Q: Why does `std::move` not actually move anything?**
    *   **A:** `std::move` is merely a compile-time cast to an rvalue reference (`T&&`). The actual "moving" of data (stealing pointers, nulling out the source) happens entirely inside the move constructor or move assignment operator of the object's class. The cast simply allows the compiler's overload resolution to select those move methods instead of the copy methods.
*   **Q: What is perfect forwarding and why do we need `std::forward`?**
    *   **A:** When a template function takes an argument as a forwarding reference (`T&& arg`), the argument `arg` inside the function has a name, making it an lvalue—even if it was passed to the function as a temporary rvalue. If we pass `arg` to another function, it would trigger a copy. `std::forward` uses reference collapsing rules to cast it back to an rvalue *only if* it was originally passed as an rvalue, thus perfectly preserving its original value category.
*   **Q: What problem does `iterator_traits` solve?**
    *   **A:** It provides a uniform way for algorithms to query properties of iterators. While custom class-based iterators can have nested typedefs (like `MyIt::value_type`), plain raw pointers (which are perfectly valid random-access iterators) cannot have nested typedefs. `iterator_traits` solves this by using template specialization to define those properties specifically for pointers, allowing the exact same algorithm code to work for both custom iterators and plain pointers.
