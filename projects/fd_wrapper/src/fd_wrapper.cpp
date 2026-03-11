#include "fd_wrapper.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace {

bool close_fd_impl(int fd) noexcept {
    if (fd < 0)
        return true;
    int r;
    do {
        r = ::close(fd);
    } while (r == -1 && errno == EINTR);
    return (r == 0);
}

void log_close_failure_impl() noexcept {
    char buf[256];
    int err = errno;
    if (strerror_r(err, buf, sizeof(buf)) != 0)
        std::strcpy(buf, "unknown error");
    const char prefix[] = "fd_wrapper: close failed: ";
    const char suffix[] = "\n";
    (void)::write(STDERR_FILENO, prefix, sizeof(prefix) - 1);
    (void)::write(STDERR_FILENO, buf, std::strlen(buf));
    (void)::write(STDERR_FILENO, suffix, sizeof(suffix) - 1);
}

} // namespace

// -----------------------------------------------------------------------------
// Constructors / destructor
// -----------------------------------------------------------------------------

fd_wrapper::fd_wrapper(const char* path) {
    int result = ::open(path, O_RDONLY | O_CLOEXEC);
    if (result >= 0)
        fd = result;
    else
        fd = INVALID;
}

fd_wrapper::fd_wrapper(int fd) noexcept {
    if (fd >= 0)
        this->fd = fd;
    else
        this->fd = INVALID;
}

fd_wrapper::fd_wrapper(fd_wrapper&& other) noexcept : fd(other.fd) {
    other.fd = INVALID;
}

fd_wrapper& fd_wrapper::operator=(fd_wrapper&& other) noexcept {
    if (this != &other) {
        swap(other);
        if (other.fd >= 0) {
            if (!close_fd(other.fd))
                log_close_failure();
            other.fd = INVALID;
        }
    }
    return *this;
}

fd_wrapper::~fd_wrapper() noexcept {
    (void)close_fd(fd);
    fd = INVALID;
}

// -----------------------------------------------------------------------------
// Private helpers (delegate to translation-unit-local impl to avoid header deps)
// -----------------------------------------------------------------------------

bool fd_wrapper::close_fd(int fd) noexcept {
    return close_fd_impl(fd);
}

void fd_wrapper::log_close_failure() noexcept {
    log_close_failure_impl();
}

// -----------------------------------------------------------------------------
// Accessors
// -----------------------------------------------------------------------------

int fd_wrapper::get_fd() const noexcept {
    if (fd >= 0)
        return fd;
    return INVALID;
}

bool fd_wrapper::valid() const noexcept {
    return fd >= 0;
}

fd_wrapper::operator bool() const noexcept {
    return valid();
}

// -----------------------------------------------------------------------------
// Lifecycle
// -----------------------------------------------------------------------------

void fd_wrapper::reset() noexcept {
    if (fd >= 0) {
        if (!close_fd(fd))
            log_close_failure();
        fd = INVALID;
    }
}

int fd_wrapper::release() noexcept {
    int f = fd;
    fd = INVALID;
    return f;
}

std::optional<fd_wrapper> fd_wrapper::open(const char* path) {
    int result = ::open(path, O_RDONLY | O_CLOEXEC);
    if (result < 0)
        return std::nullopt;
    return fd_wrapper(result);
}

void fd_wrapper::swap(fd_wrapper& other) noexcept {
    int t = fd;
    fd = other.fd;
    other.fd = t;
}

// -----------------------------------------------------------------------------
// Non-member swap
// -----------------------------------------------------------------------------

void swap(fd_wrapper& a, fd_wrapper& b) noexcept {
    a.swap(b);
}

