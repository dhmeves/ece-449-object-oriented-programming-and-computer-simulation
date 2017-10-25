// evl_wire.cpp
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

evl_wire::evl_wire () {}

evl_wire::evl_wire(std::string n, int w) : name(n), width(w) {}

// Destructors

// Setters

bool evl_wire::set(std::string n, int w) {
    //...// return false if wire is invalid
    name = n;
    width = w;
    return true;
}

bool evl_wire::set_name(std::string n) {
    //...// return false if name is invalid
    name = n;
    return true;
}

bool evl_wire::set_width(int w) {
    //...// return false if width is invalid
    width = w;
    return true;
}

// Getters

std::string evl_wire::get_name() const{
    return name;
}

int evl_wire::get_width() const{
    return width;
}

// Other Methods

bool evl_wire::process_wire_statement(evl_wires &wires, evl_statement &s) {
    //...
    enum state_type {INIT, WIRE, DONE, WIRES, WIRE_NAME, BUS, BUS_MSB, BUS_COLON, BUS_LSB, BUS_DONE};
    state_type state = INIT;
    int bus_width = 1;
    for (; !s.get_evl_tokens_ref().empty() && (state != DONE); s.get_evl_tokens_ref().pop_front()) {
        evl_token t = s.get_evl_tokens_ref().front();
        //... // use branches here to compute state transitions
        if (state == INIT) {
            //...
            if (t.get_string() == "wire") {
                state = WIRE;
                continue;
            }
            else {
                std::cerr << "Need ’wire’ but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == WIRE) {
            //...
            if (t.get_string() == "[") {
                state = BUS;
                continue;
            }
            else if (t.get_token_type() == evl_token::NAME) {
                evl_wire wire0;
                wire0.set(t.get_string(), 1);
                wires.push_back(wire0);
                state = WIRE_NAME;
                continue;
            }
            else {
                std::cerr << "Need NAME but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == WIRES) {
            //...
            //... // same as the branch for WIRE
            if (t.get_token_type() == evl_token::NAME) {
                evl_wire wire1;
                wire1.set(t.get_string(), bus_width);
                wires.push_back(wire1);
                state = WIRE_NAME;
                continue;
            }
            else {
                std::cerr << "Need NAME but found '" << t.get_string() 
                    << "' on line " << t.get_line_no() << std::endl;
                return false;
            }   
        }
        else if (state == WIRE_NAME) {
            //...
            if (t.get_string() == ",") {
                state = WIRES;
                continue;
            }
            else if (t.get_string() == ";") {
                state = DONE;
                continue;
            }
            else {
                std::cerr << "Need ’,’ or ’;’ but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == BUS) {
            if (t.get_token_type() == evl_token::NUMBER) {
                bus_width = atoi(t.get_string().c_str())+1;
                state = BUS_MSB;
                continue;
            }
        }
        else if (state == BUS_MSB) {
            if (t.get_string() == ":") {
                state = BUS_COLON;
                continue;
            }
        }
        else if (state == BUS_COLON) {
            if (t.get_string() == "0") {
                state = BUS_LSB;
                continue;
            }
        }
        else if (state == BUS_LSB) {
            if (t.get_string() == "]") {
                state = BUS_DONE;
                continue;
            }
        }
        else if (state == BUS_DONE) {
            if (t.get_token_type() == evl_token::NAME) {
                evl_wire wire2;
                wire2.set(t.get_string(), bus_width);
                wires.push_back(wire2);
                state = WIRE_NAME;
                continue;
            }
        }
    }
    if (!s.get_evl_tokens().empty() || (state != DONE)) {
        std::cerr << "something wrong with the statement" << std::endl;
        return false;
    }
    return true;
}

bool evl_wire::make_wires_table(const evl_wires &wires, evl_wires_table &wires_table) {
    for (auto &wire: wires) {
        auto same_name = wires_table.find(wire.get_name());
        if (same_name != wires_table.end()) {
            std::cerr << "Wire '" << wire.get_name()
                << "'is already defined" << std::endl;
            return false;
        }
        //wires_table[wire.name] = wire.width;
        wires_table.insert(std::make_pair(wire.get_name(), wire.get_width()));
    }
    return true;
}

bool evl_wire::make_wires_vector(const evl_wires &wires, evl_wires &wires_vector) {
    for (auto &wire: wires) {
/*        evl_wires::iterator same_name = std::find_if(wires_vector.begin(), wires_vector.end(), wire.get_name());
        if (same_name != wires_vector.end()) {
            std::cerr << "Wire '" << wire.get_name()
                << "'is already defined" << std::endl;
            return false;
        }
        //wires_table[wire.name] = wire.width;
        //wires_table.insert(std::make_pair(wire.get_name(), wire.get_width()));*/
        wires_vector.push_back(wire);
    }
    return true;
}

void evl_wire::display_wires_table(std::ostream &out, const evl_wires_table &wires_table) {
/*    for (evl_wires_table::const_iterator it = wires_table.begin();
        it != wires_table.end(); ++it) {
        out << "wire " << it->first
            << " " << it->second << std::endl;
    }*/
    for (auto &kv: wires_table) {
        out << "wire " << kv.first
            << " " << kv.second << std::endl;
    }
}

void evl_wire::display_wires_vector(std::ostream &out, const evl_wires &wires_vector) {
/*    for (evl_wires_table::const_iterator it = wires_table.begin();
        it != wires_table.end(); ++it) {
        out << "wire " << it->first
            << " " << it->second << std::endl;
    }*/
    for (auto &kv: wires_vector) {
        out << "wire " << kv.get_name()
            << " " << kv.get_width() << std::endl;
    }
}
