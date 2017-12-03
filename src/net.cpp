// net.cpp

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
#include <exception>

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

net::net() {}

net::net(std::string n) : name_(n) {}

net::net(std::string n, char s, std::list<pin *> c) : name_(n), signal_(s), connections_(c) {}

// Destructors

// Setters

bool net::set(std::string n, char s, std::list<pin *> c) {
    //...// return false if net is invalid
    name_ = n;
    signal_ = s;
    connections_ = c;
    return true;
}

bool net::set_name_(std::string n) {
    //...// return false if name is invalid
    name_ = n;
    return true;
}

bool net::set_signal_(char s) {
    //...// return false if signal is invalid
    signal_ = s;
    return true;
}

bool net::set_connections_(std::list<pin *> c) {
    //...// return false if connections are invalid
    connections_ = c;
    return true;
}

// Getters

std::string net::get_name_() const {
    return name_;
}

char net::get_signal_() const {
    return signal_;
}

std::list<pin *> net::get_connections_() const {
    return connections_;
}

std::list<pin *> & net::get_connections_ref() {
    return connections_;
}

// Other Methods

void net::append_pin(pin *p) {
    connections_.push_back(p);
}

// project 4?

char net::get_signal() {
    if (signal_ == '?') {
        auto it = std::find_if(connections_.begin(), connections_.end(), [](pin *p) {return p->get_dir_() == '0';});
        if (it == connections_.end())
            throw std::runtime_error("floating net");
        pin *driver = *it;
        signal_ = driver->compute_signal();
    }
    return signal_;
}
