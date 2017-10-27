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
//#include "Vec.hpp"

// Constructors

    pin::pin() {}

    pin::pin(gate *g, size_t i) : gate_(g), index_(i) {}

    pin::pin(char d, gate *g, size_t i, net *n, int w) : dir_(d), gate_(g), index_(i), net_(n), width_(w) {}

// Destructors

// Setters

    bool pin::set(char d, gate *g, size_t i, net *n, int w) {
        //...// return false if pin is invalid
        dir_ = d;
        gate_ = g;
        index_ = i;
        net_ = n;
        width_ = w;
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

/*    bool pin::set_nets_(std::list<net *> n) {
        //...// return false if net is invalid
        nets_ = n;
        return true;
    }*/

    bool pin::set_net_ptr(net *n) {
        //...// return false if net is invalid
        net_ = n;
        return true;
    }

    bool pin::set_width_(int w) {
        //.../ return false if width is invalid
        width_ = w;
        return true;
    }

    bool pin::set_as_input() {
        set_dir_('I');
        return true;
    }

    bool pin::set_as_output() {
        set_dir_('O');
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

/*    std::list<net *> pin::get_const_net_ptr() const {
        return nets_;
    }

    std::list<net *> pin::get_net_ptr() {
        return nets_;
    } */

    net * pin::get_const_net_ptr() const {
        return net_;
    }

    net * pin::get_net_ptr() {
        return net_;
    }

    int pin::get_width_() const {
        return width_;
    }

// Other Methods

/*bool pin::calculate_width(const evl_pin &p) {
    width_ = 0;
    int msb = p.get_bus_msb();
    int lsb = p.get_bus_lsb();
    if ((msb == -1) && (lsb == -1)) 
        width_ = 1;
    else if ((msb != -1) && (lsb == -1)) 
        width_ = msb + 1;
    else if ((msb != -1) && (lsb != -1))
        width_ = msb - lsb + 1;
    return true;
}*/

bool pin::create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net *> &nets_table) {
    // store g and index;
    gate_ = g;
    index_ = index;
    if (p.get_bus_msb() == -1) { // a 1-bit wire
        std::string net_name = p.get_name();
        net_ = nets_table.find(net_name)->second;
        net_->append_pin(this);
    }
    else {  // a bus
        // ... //
/*        if ((p.get_bus_msb() == -1) && (p.get_bus_lsb() == -1)) {
            auto bus_name = p.get_name();
            auto bus_ = nets_table.find(bus_name);
            if (bus_ != nets_table.end()) {
                bus_->second->append_pin(this);
            }
            else {
                std::cerr << "BUS not found in nets table" << std::endl;
                return false;
            }
        }
        else if ((p.get_bus_msb() != -1) && (p.get_bus_lsb() != -1)) {
            assert(p.get_bus_lsb() >= 0);
            assert(p.get_bus_msb() >= p.get_bus_lsb());
            assert(width_ >= p.get_bus_msb());
            auto both_name = p.get_name(); 
            auto both_ = nets_table.find(both_name);
            if (both_ != nets_table.end()) {
                both_->second->append_pin(this);
            }
            else {
                std::cerr << "BOTH not found in nets table" << std::endl;
                return false;
            }
        }
        else if ((p.get_bus_msb() != -1) && (p.get_bus_lsb() == -1)) {
            assert(p.get_bus_msb() >= 0);
            assert(width_ >= p.get_bus_msb());
            auto bit_name = p.get_name();
            auto bit_ = nets_table.find(bit_name);
            if (bit_ != nets_table.end()) {
                bit_->second->append_pin(this);
            }
            else {
                std::cerr << "BIT not found in nets table" << std::endl;
                return false;
            }
        }
        else {
            return false;
       }*/
    }
    return true;
}

// project 4
/*
char pin::compute_signal() {
    if (dir_ == '0')
        return gate_->compute_signal(index_);
    else // dir_ == ’I’
        return net_->get_signal_();
}
*/
