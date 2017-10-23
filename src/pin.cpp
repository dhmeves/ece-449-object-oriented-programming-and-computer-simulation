// pin.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <map>
#include <unordered_map>

#include "lex.hpp"
#include "evl_token.hpp"
#include "evl_statement.hpp"
#include "evl_wire.hpp"
#include "evl_component.hpp"
#include "evl_pin.hpp"
#include "netlist.hpp"
#include "net.hpp"
#include "gate.hpp"
#include "pin.hpp"
#include "Vec.hpp"

// Constructors

    pin::pin() {}

    pin::pin(char d, gate *g, size_t i, net *n) : dir_(d), gate_(g), index_(i), net_(n) {}

// Destructors

// Setters

    bool pin::set(char d, gate *g, size_t i, net *n) {
        //...// return false if pin is invalid
        dir_ = d;
        gate_ = g;
        index_ = i;
        net_ = n;
        return true;
    }

    bool pin::set_dir_(char d) {
        //...// return false if dir is invalid
        dir_ = d;
        return true;
    }

    bool pin::set_gate_ptr(gate *g) {
        //...// return false if gate is invalid
        gate_ = g;
        return true;
    }

    bool pin::set_index_(size_t i) {
        //...// return false if index is invalid
        index_ = i;
        return true;
    }

    bool pin::set_net_ptr(net *n) {
        //...// return false if net is invalid
        net_ = n;
        return true;
    }

// Getters

    char pin::get_dir_() const {
        return dir_;
    }

    gate * pin::get_const_gate_ptr() const {
        return gate_;
    }

    gate * pin::get_gate_ptr() {
        return gate_;
    }

    size_t pin::get_index_() const {
        return index_;
    }

    net * pin::get_const_net_ptr() const {
        return net_;
    }

    net * pin::get_net_ptr() {
        return net_;
    }

// Other Methods
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

char pin::compute_signal() {
    if (dir_ == ’O’)
        return gate_->compute_signal(index_);
    else // dir_ == ’I’
        return net_->get_signal();
}
