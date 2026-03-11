#include "fd_wrapper.hpp"
#include <cstdio>
#include <iostream>

int main() {
    auto w = fd_wrapper::open("/dev/null");
    if (!w) {
        perror("open");
        return 1;
    }
    std::cout << "opened /dev/null, fd=" << w->get_fd() << "\n";

    fd_wrapper other = std::move(*w);
    (void)other;

    std::cout << "fd_wrapper demo ok\n";
    return 0;
}
