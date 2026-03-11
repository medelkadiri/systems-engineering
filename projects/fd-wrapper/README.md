# fd-wrapper

A small, **RAII C++17 wrapper** for POSIX file descriptors. Move only, exception safe, and designed for use in systems and low level code where correct resource lifetime and error handling is important.

## Why it exists

Raw file descriptors are easy to leak or double close. This wrapper:

- **Closes the fd in the destructor** so you don’t forget.
- Is **move only** (no copy) so ownership is explicit.
- Uses **EINTR safe close** and **O_CLOEXEC** by default.
- Exposes a **factory** that returns `std::optional<fd_wrapper>` so open failures are explicit and don’t throw.

Suitable for utilities, daemons, and any C++ code that talks to the POSIX API and aims for clear, maintainable resource handling.

## Features

- **RAII** : Destructor closes the fd, no manual `close()` in normal flow.
- **Move only** : Copy is disabled, move constructor and move assignment transfer ownership. 
- **EINTR safe close** : Close is retried on `EINTR`, no silent failure from signals.
- **O_CLOEXEC** : Opened fds are close on exec by default.
- **Optional factory** : `fd_wrapper::open(path)` returns `std::optional<fd_wrapper>` which checks before use.
- **Explicit failure** : Path constructor leaves the object invalid on failure, use `valid()` / `operator bool()` and errno. 
- **reset() / release()** : Close and invalidate, or release ownership to the caller.
- **swap** : Member and non member `swap` for clear ownership transfer.

## Requirements

- **C++17** or later  
- **POSIX** environment (Linux, macOS, BSD, etc.)  
- **CMake 3.16+** for building  

## Build and test

From the project root:

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

Run the demo:

```bash
./fd_wrapper_demo
```

Run tests (registered with CTest):

```bash
ctest --output-on-failure
```

Or run the test binary directly:

```bash
./fd_wrapper_tests
```

## Usage

```cpp
#include "fd_wrapper.hpp"

// Factory: explicit error handling
auto w = fd_wrapper::open("/etc/hostname");
if (!w) {
    // open failed, check errno
    return;
}
read(w->get_fd(), buf, size);

// Adopt an existing fd
int fd = ::open(path, O_RDONLY | O_CLOEXEC);
fd_wrapper guard(fd);  // takes ownership

// Move
fd_wrapper other = std::move(guard);

// Release ownership back to caller (caller must close)
int raw = guard.release();

// Check validity
if (w->valid()) { /* ... */ }
```

## Project layout

```
fd_wrapper/
├── README.md
├── CMakeLists.txt
├── include/
│   └── fd_wrapper.hpp    # Public API (declarations only)
├── src/
│   ├── fd_wrapper.cpp    # Implementation
│   └── demo.cpp          # Example program
├── tests/
    └── fd_wrapper_tests.cpp

```

## Design notes

- **No exceptions** in the API: destructor, `reset()`, and close helpers are `noexcept`. Failures are reported via return values and errno.
- **Close failures** are logged to stderr (e.g. after EINTR retries are exhausted). Tthe object still releases the fd so the process can continue.
- **Invalid fd** is represented by `fd_wrapper::INVALID` (-1).`valid()` and `explicit operator bool()` allow simple checks.
