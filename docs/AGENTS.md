# AGENTS.md

## Project Vision

Build a mini STL-like library from scratch to:

- master modern C++
- strengthen data structure knowledge
- understand memory management
- prepare for C++ interviews
- challenge personal engineering skills

This project is strictly for learning purposes.

This project is NOT intended for production usage.

---

## Core Philosophy

Focus only on concepts that matter for:

- engineering fundamentals
- interview discussions
- practical implementation thinking

Do NOT spend time on highly specialized STL internals that are:

- overly complex
- compiler-specific
- rarely discussed in interviews
- low learning ROI

Examples to avoid:

- allocator internals
- PMR
- advanced template metaprogramming
- ABI compatibility
- implementation-specific micro optimizations

Readable implementation > clever implementation

Correctness > optimization

Learning value > STL perfection

Interview explainability > implementation complexity

---

# Priority Order

1. Interview showcase
2. Personal challenge
3. Deep STL understanding
4. Clean architecture

---

# Scope

## Must Have Containers

### Sequential Containers
- vector
- string (no SSO)
- list
- deque

### Container Adapters
- stack
- queue
- priority_queue

### Tree-based Containers
- Binary Search Tree
- map (built on BST)
- set (built on BST)

### Hash-based Containers
- HashTable (separate chaining)
- unordered_map
- unordered_set

---

## Must Have Algorithms
- find
- copy
- remove
- lower_bound

---

## Must Have Smart Pointers
- unique_ptr
- shared_ptr
- weak_ptr
- custom deleter support

---

## Must Have Utilities
- pair
- move
- forward
- swap
- iterator_traits

---

## Nice To Have (Optional Future Expansion)

- Red Black Tree
- More STL algorithms
- Better hash resizing strategies
- Better exception handling
- Additional benchmarks
- Performance tuning

---

# API Philosophy

Public APIs must intentionally mirror `std::` as closely as possible.

Examples:

```cpp
gstd::vector<int> nums;
nums.push_back(10);
nums.push_back(20);

auto it = nums.begin();
gstd::unordered_map<int, std::string> table;
table.insert({1, "hello"});
```
Rules:

- Keep public type names identical to STL naming:
    - vector
    - string
    - list
    - map
    - unordered_map
- Keep public methods identical to STL naming:
    - push_back()
    - pop_back()
    - begin()
    - end()
    - insert()
    - erase()
    - size()
    - reserve()

The only intentional difference from STL:
- namespace = gstd

This enables:
- familiar developer experience
- easier benchmarking with std
- better interview discussion
- easy migration between gstd and std

Avoid unnecessary API deviations unless implementation complexity becomes too high.

# Internal Naming Rules

Public API:

- Follow STL naming exactly

Internal implementation:

- Use readable names for maintainability

Examples:

- TreeNode
- HashNode
- ControlBlock
- MemoryManager
- BucketEntry

Internal readability is more important than forcing STL naming everywhere.

---

# Architecture Rules

Build reusable primitives first.

Dependency flow:

Utilities
    ↓

Iterators
    ↓

Vector → Stack

List → Queue

Heap → PriorityQueue

BST → Map/Set

HashTable → UnorderedMap/UnorderedSet

ControlBlock → SharedPtr/WeakPtr

Algorithms depend on iterators + containers

---

# Iterator Rules

Required:
- Forward iterator

Optional when naturally needed:
- Bidirectional iterator (List)
- Random access iterator (Vector)

Do not overbuild iterator abstraction.

---

# Error Handling

Use assert-based validation only.

Examples:

- invalid access
- null dereference
- invalid iterator operations

Do NOT spend time on:

- full exception guarantees
- complex exception propagation logic

---

# Testing Strategy

Every module must include:

- Unit tests
- Edge case tests
- Iterator tests
- Copy/move tests
- Memory leak validation

Tools:

- GoogleTest
- AddressSanitizer
- Valgrind

---

# Benchmarking

Benchmark selected containers against STL equivalents:

Examples:

- gstd::vector vs std::vector
- gstd::map vs std::map
- gstd::unordered_map vs std::unordered_map

Purposes:
- learning
- performance awareness
- interview discussion

Not for premature optimization.

---

# Documentation

Use markdown docs for every major module:

- implementation notes
- tradeoffs
- limitations
- API examples
- lessons learned

Examples:

- docs/vector.md
- docs/map.md
- docs/shared_ptr.md

---

# Build Environment

- Ubuntu 22.04
- GCC/G++
- CMake
- C++17

---

# Definition of Done

A module is complete when:

- Code compiles
- Tests pass
- No memory leaks
- API works as expected
- Edge cases handled
- Benchmark completed (if applicable)
- Markdown documentation updated
- PLAN.md progress updated

---

# Absolute Anti-Goals

Do NOT become:

- libstdc++ engineer simulator
- template metaprogramming rabbit hole
- premature optimization project
- unreadable abstraction-heavy codebase

This project exists to make you a stronger software engineer.