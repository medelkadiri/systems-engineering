# C++17 systems and concurrency practice

A set of small C++17 projects: allocators, concurrent data structures, and utilities. Each subdirectory is one project, build and run from inside that directory.

| Project | Description |

| [async-logger](async-logger/) | Asynchronous logger with lock-free queue and dedicated logging thread |
| [bounded-queue](bounded-queue/) | Thread-safe bounded blocking queue (condition variables + mutex) |
| [copy-on-write-string](copy-on-write-string/) | Copy-on-write string with shared memory and deep copy on mutation |
| [custom-function](custom-function/) | Type-erased callable wrapper with small-buffer optimization |
| [fd-wrapper](fd-wrapper/) | RAII wrapper for POSIX file descriptors (move-only, exception-safe) |
| [hash-map](hash-map/) | Hash map with open addressing or chaining, rehashing, iterator validity |
| [lock-free-stack](lock-free-stack/) | Lock-free stack with CAS and ABA mitigation (e.g. version tagging) |
| [lru-cache](lru-cache/) | Thread-safe LRU cache, O(1) get/put (hash map + doubly linked list) |
| [memory-pool](memory-pool/) | Fixed-size memory pool allocator, O(1) alloc/free, alignment |
| [ring-buffer](ring-buffer/) | Lock-free SPSC ring buffer for low-latency single producer/consumer |
| [singleton-dcl](singleton-dcl/) | Double-checked locking singleton with C++11 atomics |
| [spin-lock](spin-lock/) | Spin lock using `atomic_flag`; compare with mutex and benchmark |
| [thread-pool](thread-pool/) | Thread pool with task submission, graceful shutdown, work stealing |
| [vector-impl](vector-impl/) | Simplified `Vector<T>`: dynamic buffer, reserve, resize, copy/move |

## Requirements

- C++17
- CMake 3.16+
- POSIX (for `fd_wrapper` and any project using file I/O)

## Building a single project

From this directory:

```bash
cd <project_name>
mkdir -p build && cd build
cmake ..
cmake --build .
```

## Repo layout

Each project is self-contained under its own folder (e.g. `include/`, `src/`, `tests/`, `CMakeLists.txt`, `README.md`). Some are stubs to be implemented later.

