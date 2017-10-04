// evl_wire.cpp
#include "lex.hpp"
#include "evl_wire.hpp"
#include "evl_statement.hpp"
#include <iostream>

evl_wire::evl_wire () {}

evl_wire::evl_wire(std::string n, int w) : name(n), width(w) {
}

bool set(std::string n, int w) {
    //...// return false if wire is invalid
    name = n;
    width = w;
    return true;
}

bool set_name(std::string n) {
    //...// return false if name is invalid
    name = n;
    return true;
}

bool set_width(int w) {
    //...// return false if width is invalid
    width = w;
    return true;
}

std::string get_name() const{
    return name;
}

int get_width() const{
    return width;
}

bool evl_wire::process_wire_statement(evl_wires &wires, evl_statement &s) {
    //...
    enum state_type {INIT, WIRE, DONE, WIRES, WIRE_NAME};
    state_type state = INIT;
    for (; !s.get_evl_tokens().empty() && (state != DONE); s.get_evl_tokens().pop_front()) {
        evl_token t = s.get_evl_tokens().front();
        //... // use branches here to compute state transitions
        if (state == INIT) {
            //...
            if (t.get_string() == "wire") {
                state = WIRE;
            }
            else {
                std::cerr << "Need ’wire’ but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == WIRE) {
            //...
            if (t.get_token_type() == evl_token::NAME) {
                evl_wire wire;
                wire.set(t.get_string(), 1);
                wires.push_back(wire);
                state = WIRE_NAME;
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
        }
        else if (state == WIRE_NAME) {
            //...
            if (t.get_string() == ",") {
                state = WIRES;
            }
            else if (t.get_string() == ";") {
                state = DONE;
            }
            else {
                std::cerr << "Need ’,’ or ’;’ but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
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

void evl_wire::display_wires_table(std::ostream &out, const evl_wires_table &wires_table) {
    for (evl_wires_table::const_iterator it = wires_table.begin();
        it != wires_table.end(); ++it) {
        out << "wire " << it->first
            << " " << it->second << std::endl;
    }
}
