#include "fd_wrapper.hpp"
#include <cassert>
#include <unistd.h>
#include <fcntl.h>

int main() {
    fd_wrapper empty;
    assert(!empty.valid());
    assert(empty.get_fd() == fd_wrapper::INVALID);

    int raw = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    assert(raw >= 0);
    fd_wrapper g(raw);
    assert(g.valid());
    assert(g.get_fd() == raw);
    g.reset();
    assert(!g.valid());

    auto opt = fd_wrapper::open("/dev/null");
    assert(opt && opt->valid());

    fd_wrapper a = fd_wrapper::open("/dev/null").value();
    assert(a.valid());
    fd_wrapper b = std::move(a);
    assert(!a.valid());
    assert(b.valid());
    int fd = b.release();
    assert(fd >= 0);
    assert(!b.valid());
    ::close(fd);

    fd_wrapper x = fd_wrapper::open("/dev/null").value();
    fd_wrapper y;
    swap(x, y);
    assert(!x.valid());
    assert(y.valid());

    return 0;
}
