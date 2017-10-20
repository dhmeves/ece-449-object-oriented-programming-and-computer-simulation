// evl_pin.cpp
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

evl_pin::evl_pin() {}

evl_pin::evl_pin(std::string n, int msb, int lsb) : name(n), bus_msb(msb), bus_lsb(lsb) {}

bool evl_pin::set(std::string n, int msb, int lsb) {
    //...// return false if pin is not valid
    name = n;
    bus_msb = msb;
    bus_lsb = lsb;
    return true;
}

bool evl_pin::set_name(std::string n) {
    //...// return false if pin is not valid
    name = n;
    return true;
}

bool evl_pin::set_bus_msb(int msb) {
    //...// return false if pin is not valid
    bus_msb = msb;
    return true;
}

bool evl_pin::set_bus_lsb(int lsb) {
    //...// return false if pin is not valid
    bus_lsb = lsb;
    return true;
}

std::string evl_pin::get_name() const{
    return name;
}

int evl_pin::get_bus_msb() const{
    return bus_msb;
}

int evl_pin::get_bus_lsb() const{
    return bus_lsb;
}
