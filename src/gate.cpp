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

void gate::compute_next_state_or_output() {
    if (type_ == "evl_dff") {
        next_state_ = pins_[1]->compute_signal(); // d
    }
    else if (type_ == "evl_output") {
        collect signal from all pins and write to file
    }
}

char gate::compute_signal(int pin_index) {
    if (type_ == "evl_dff") {
        assert pin_index == 0; // must be q
        return state_;
    }
    else if (type_ == "evl_zero") {
        return ’0’;
    }
    else if (type_ == "and") {
        assert pin_index == 0; // must be out
        collect signals from the input pins
        compute and return the output signal
    }
    //...//
}

bool gate::validate_structural_semantics() {
    if (type_ == "and") {
        if (pins_.size() < 3) return false;
        pins_[0]->set_as_output(); // out
        for (size_t i = 1; i < pins_.size(); ++i)
            pins_[i]->set_as_input();
    }
    else if (type_ == "evl_dff") {
        if (pins_.size() != 3) return false;
        pins_[0]->set_as_output(); // q
        pins_[1]->set_as_input(); // d
        pins_[2]->set_as_input(); // clk
    }
    //...//
}

