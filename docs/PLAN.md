# PLAN.md

## Project Goal

Build a mini STL-like library (`gstd`) from scratch.

Final outcomes:

* Understand how STL containers work internally
* Improve modern C++ implementation skills
* Strengthen DSA knowledge
* Improve memory management understanding
* Create strong interview discussion material
* Practice decomposing a large engineering project

This roadmap is intentionally focused on:

* learning fundamentals deeply
* implementing by yourself
* avoiding low-ROI STL internals

---

## Estimated Timeline

**8–12 weeks**

Why not 1 month?

Your scope includes:

* containers
* iterators
* algorithms
* smart pointers
* hashing
* trees
* testing
* benchmarking
* documentation

Doing all of that properly in 1 month would likely force shallow implementation.

---

# Development Workflow (Repeat For Every Module)

For each module:

1. Learn theory
2. Study std behavior
3. Design API
4. Design internal structure
5. Write pseudocode
6. Implement MVP
7. Write tests
8. Debug
9. Benchmark vs std
10. Write markdown notes
11. Move forward

---

# Suggested Project Structure

```cpp
 gstd/
 ├── include/
 │   └── gstd/
 │       ├── vector.hpp
 │       ├── string.hpp
 │       ├── list.hpp
 │       ├── deque.hpp
 │       ├── map.hpp
 │       ├── unordered_map.hpp
 │       ├── algorithm/
 │       ├── memory/
 │       └── utility/
 │
 ├── tests/
 ├── benchmarks/
 ├── docs/
 └── CMakeLists.txt
```

---

# PHASE 0 — Foundation Setup (2–3 days)

## Learn

Review:

* templates
* RAII
* rule of five
* copy semantics
* move semantics
* references

## Setup

* CMake
* GoogleTest
* AddressSanitizer
* Valgrind
* benchmark folder

## Deliverables

* project builds
* tests run successfully
* debugging environment ready

## Self Review Questions

* Why does RAII matter?
* When should move semantics be used?
* Why is rule of five important?

---

# PHASE 1 — Utilities (3–5 days)

Build reusable foundations first.

## Implement

### pair

Learn:

* template basics
* constructor forwarding

---

### move

Learn:

* rvalue references
* static_cast

---

### forward

Learn:

* perfect forwarding (conceptually)

---

### swap

Learn:

* move-based swap

---

### iterator_traits

Learn:

* generic programming fundamentals

---

## Deliverables

* gstd::pair
* gstd::move
* gstd::forward
* gstd::swap
* gstd::iterator_traits

## Self Review Questions

* Why does std::move not move anything?
* What problem does iterator_traits solve?

---

# PHASE 2 — vector (1–2 weeks)

Most important container.

## Learn First

* dynamic arrays
* capacity growth
* amortized complexity
* iterator invalidation

## Implement

* constructors
* destructor
* push_back
* pop_back
* reserve
* resize
* operator[]
* begin
* end
* clear
* size
* capacity

## Focus

Understand:

* memory ownership
* reallocation
* copy vs move behavior

## Tests

* push/pop
* resize
* iteration
* copy
* move
* edge cases

## Benchmark

* gstd::vector vs std::vector

## Self Review Questions

* Why is push_back amortized O(1)?
* Why does reallocation invalidate iterators?

---

# PHASE 3 — string (4–6 days)

## Learn

* dynamic char storage
* null termination

## Implement

* constructors
* copy/move
* append
* c_str
* operator[]
* size

## Explicitly Ignore

* SSO
* encoding complexity

## Self Review Questions

* Why is string harder than vector<char>?

---

# PHASE 4 — list (1 week)

## Learn

* linked list tradeoffs

## Implement

* node
* push_front
* push_back
* pop_front
* pop_back
* insert
* erase

## Iterator

* forward iterator required
* bidirectional iterator optional

## Self Review Questions

* Why does list often perform worse than vector in real systems?

---

# PHASE 5 — stack + queue (2–3 days)

## Build On Top Of

* vector
  n- list
* deque

## Implement

* push
* pop
* top
* front
* back

## Self Review Questions

* Why are adapters useful?

---

# PHASE 6 — deque (1 week)

## Learn

* segmented arrays

## Implement

Simplified version only.

Do NOT replicate libstdc++ complexity.

## Self Review Questions

* Why does deque exist?

---

# PHASE 7 — Heap + priority_queue (4–5 days)

## Learn

* binary heap

## Implement

* heapify
* push
* pop

## Build

* priority_queue

## Self Review Questions

* Why is heap different from sorted array?

---

# PHASE 8 — BST → map/set (1–2 weeks)

## Learn

* insertion
* deletion
* traversal

## Implement

### BST

Core operations first.

### map

Built on BST.

### set

Built on BST.

## Future Expansion

* Red Black Tree

## Self Review Questions

* Why can BST degrade?
* Why does std::map use RB Tree?

---

# PHASE 9 — HashTable → unordered containers (1–2 weeks)

## Learn

* hashing
* collisions
* load factor

## Implement

Use:

* separate chaining

## Build

* unordered_map
* unordered_set

## Self Review Questions

* Why can hash tables degrade to O(n)?

---

# PHASE 10 — Algorithms (4–6 days)

## Implement

* find
* copy
* remove
* lower_bound

## Focus

Understand why algorithms should be independent of containers.

## Self Review Questions

* Why are iterators critical for generic algorithms?

---

# PHASE 11 — Smart Pointers (1–2 weeks)

## unique_ptr

Learn:

* exclusive ownership

Implement:

* move-only semantics
* custom deleter

---

## shared_ptr

Learn:

* reference counting
* control block

Implement:

* shared ownership

---

## weak_ptr

Learn:

* weak ownership
* cyclic references

Implement:

* lock()

## Self Review Questions

* Why do cyclic references happen?
* Why is weak_ptr needed?

---

# PHASE 12 — Final Cleanup (1 week)

## Refactor

* remove duplication
* improve naming
* improve APIs

## Documentation

Write:

* implementation decisions
* tradeoffs
* limitations
* lessons learned

## Final Benchmarks

Compare:

* gstd::vector vs std::vector
* gstd::map vs std::map
* gstd::unordered_map vs std::unordered_map

---

# Must Have vs Nice To Have

## Must Have

* vector
* string
* list
* deque
* stack
* queue
* priority_queue
* map
* set
* unordered_map
* unordered_set
* algorithms
* smart pointers
* utilities

## Nice To Have

* Red Black Tree
* more algorithms
* deeper optimization
* stronger hash resizing

---

# Definition of Done (Per Module)

A module is complete when:

* code compiles
* tests pass
* no memory leaks
* API matches std style
* benchmark completed
* markdown docs updated

---

# How To Use AI Correctly

Good prompts:

* review my design
* debug this issue
* compare tradeoffs
* explain complexity

Bad prompts:

* write full implementation for me

The goal is:

**you build it yourself.**

---

# Interview Preparation Layer

Be ready to explain:

* why you chose each data structure
* complexity tradeoffs
* memory ownership models
* iterator invalidation
* hash collisions
* tree balancing limitations

Example strong interview statement:

> I rebuilt a simplified STL clone from scratch including containers, iterators, hashing, trees, smart pointers, algorithms, testing, and benchmarking.

That is the real value of this project.
