// gate.cpp
#include "gate.hpp"

bool gate::create(const evl_component &c, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table) {
    // set gate type and name;
    size_t index = 0;
    for each evl_pin ep in c {
        create_pin(ep, index, nets_table, wires_table);
        ++index;
    }
    return validate_structural_semantics();
}

bool gate::create_pin(const evl_pin &ep, size_t index, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table) {
    // resolve semantics of ep using wires_table
    pin *p = new pin;
    pins_.push_back(p);
    return p->create(this, index, ep, nets_table);
}

gate::~gate() {
    for (size_t i = 0; i < pins_.size(); ++i)
        delete pins_[i];
}
