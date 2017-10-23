// net.cpp
#include "net.hpp"

void net::append_pin(pin *p) {
    connections_.push_back(p);
}

char net::get_signal() {
    if (signal_ == ’?’) {
        auto it = std::find_if(connections_.begin(), connections_.end(),
            [](pin *p) {p->get_dir() == ’O’;});
        if (it == connections_.end())
            throw std::runtime_exception("floating net");
        pin *driver = *it;
        signal_ = driver->compute_signal();
    }
    return signal_;
}

