#pragma once

#include <optional>

/**
 * RAII wrapper for a POSIX file descriptor.
 *
 * - Move only, copy is disabled.
 * - EINTR safe close, logs to stderr on close failure (no throw).
 * - INVALID (fd=-1) represents no, fd valid() / operator bool() for checks.
 */
class fd_wrapper {
public:
    static constexpr int INVALID = -1;

    /// Upper bound for valid fd values. Platform-specific for maximum
    /// security. Any fd above this is rejected as corrupted data.
    ///
    /// Tighter bounds catch corruption earlier. Each platform uses
    /// its own kernel maximum as the ceiling.
#if defined(__linux__)
    static constexpr int MAX_VALID_FD = 1 << 20;   // 1,048,576 (Linux /proc/sys/fs/nr_open)
#elif defined(__APPLE__) && defined(__MACH__)
    static constexpr int MAX_VALID_FD = 10 << 20;  // 10,485,760 (macOS kern.maxfilesperproc max)
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    static constexpr int MAX_VALID_FD = 1 << 20;   // 1,048,576 (BSD default)
#else
    #error "Unsupported platform: define MAX_VALID_FD for your OS"
#endif

    // --- Constructors -------------------------------------------------------
    fd_wrapper() noexcept = default;

    /// Open by path (O_RDONLY | O_CLOEXEC). On failure, fd is INVALID check valid() and errno.
    explicit fd_wrapper(const char* path);

    /// Take ownership of an existing fd. Invalid fds are stored as INVALID.
    explicit fd_wrapper(int fd) noexcept;

    fd_wrapper(const fd_wrapper&) = delete;
    fd_wrapper& operator=(const fd_wrapper&) = delete;

    fd_wrapper(fd_wrapper&& other) noexcept;
    fd_wrapper& operator=(fd_wrapper&& other) noexcept;

    ~fd_wrapper() noexcept;

    // --- Accessors -----------------------------------------------------------
    int get_fd() const noexcept;
    bool valid() const noexcept;
    explicit operator bool() const noexcept;

    // --- Lifecycle ----------------------------------------------------------
    /// Close and set to INVALID. Logs to stderr on close failure.
    void reset() noexcept;

    /// Transfer ownership to caller, wrapper becomes empty. Caller must close or adopt.
    int release() noexcept;

    /// Factory: returns std::nullopt on open failure.
    static std::optional<fd_wrapper> open(const char* path);

    void swap(fd_wrapper& other) noexcept;

private:
    int fd = INVALID;

    static bool is_valid_fd_value(int fd) noexcept;
    static bool close_fd(int fd) noexcept;
    static void log_close_failure() noexcept;
};

/// Non member swap for ADL and generic code.
void swap(fd_wrapper& a, fd_wrapper& b) noexcept;

