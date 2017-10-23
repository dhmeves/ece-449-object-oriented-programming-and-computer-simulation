// pin.cpp
#include "pin.hpp"

bool pin::create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net *> &nets_table) {
    // store g and index;
    if (p.msb == -1) { // a 1-bit wire
        net_name = p.name;
        net_ = find net_name in nets_table
        net_->append_pin(pin);
    }
    else { // a bus
       // ... //
    }
}
