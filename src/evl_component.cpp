// evl_component.cpp
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

evl_component::evl_component() {}

evl_component::evl_component(std::string t, std::string n, evl_pins pv) : type(t), name(n), pin_vector(pv) {}

// Destructors

// Setters

bool evl_component::set(std::string t, std::string n, evl_pins pv) {
    //...// return false if component is not valid
    type = t;
    name = n;
    pin_vector = pv;
    return true;
}

bool evl_component::set_type(std::string t) {
    //...// return false if component is not valid
    type = t;
    return true;
}

bool evl_component::set_name(std::string n) {
    //...// return false if component is not valid
    name = n;
    return true;
}

bool evl_component::set_pin_vector(evl_pins pv) {
    //,,,// return false if component is not valid
    pin_vector = pv;
    return true;
}

// Getters

std::string evl_component::get_type() const {
    return type;
}

std::string evl_component::get_name() const {
    return name;
}

evl_pins evl_component::get_pin_vector() const{
    return pin_vector;
}

evl_pins & evl_component::get_pin_vector_ref() {
    return pin_vector;
}

// Other Methods

bool evl_component::process_component_statement(evl_statement &s) {
    //...
    enum state_type {INIT, TYPE, DONE, NAME, PINS, PIN_NAME, PINS_DONE, BUS, BUS_MSB, BUS_COLON, BUS_LSB, BUS_DONE};
    state_type state = INIT;
    evl_pins pin_vec;
    evl_pin pin;
    for (; !s.get_evl_tokens_ref().empty() && (state != DONE); s.get_evl_tokens_ref().pop_front()) {
        evl_token t = s.get_evl_tokens_ref().front();
        //... // use branches here to compute state transitions
        if (state == INIT) {
            //...
            if (t.get_token_type() == evl_token::NAME) {
                s.get_evl_component_ref().set(t.get_string(), "", pin_vec);            
                state = TYPE;
                continue;
            }
            else {
                std::cerr << "Need ’NAME’ but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == TYPE) {
            //...
            if (t.get_string() == "(") {
                state = PINS;
                continue;
            }
            else if (t.get_token_type() == evl_token::NAME) {
                s.get_evl_component_ref().set_name(" "+t.get_string());
                state = NAME;
                continue;
            }
            else {
                std::cerr << "Need NAME but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == NAME) {
            //...
            //... // same as the branch for WIRE
            if (t.get_string() == "(") {
                state = PINS;
                continue;
            }
            else {
                std::cerr << "Need ( but found '" << t.get_string() 
                    << "' on line " << t.get_line_no() << std::endl;
                return false;
            }   
        }
        else if (state == PINS) {
            //...
            if (t.get_token_type() == evl_token::NAME) {
                pin.set(t.get_string(), -1, -1);
                state = PIN_NAME;
                continue;
            }
            else {
                std::cerr << "Need NAME but found ’" << t.get_string()
                    << "’ on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == PIN_NAME) {
            if (t.get_string() == "[") {
                state = BUS;
                continue;
            }
            else if (t.get_string() == ")") {
                s.get_evl_component_ref().get_pin_vector_ref().push_back(pin);                
                state = PINS_DONE;
                continue;
            }
            else if (t.get_string() == ",") {
                s.get_evl_component_ref().get_pin_vector_ref().push_back(pin);
                state = PINS;
                continue;
            }
            else {
                std::cerr << "Need '[' or ')' or ',' but found '" << t.get_string()
                    << "' on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == BUS) {
            if (t.get_token_type() == evl_token::NUMBER) {
                pin.set_bus_msb(atoi(t.get_string().c_str()));
                state = BUS_MSB;
                continue;
            }
            else {
                std::cerr << "Need NUMBER but found '" << t.get_string()
                    << "' on line " << t.get_line_no() << std::endl;
                return false;
            }
        }
        else if (state == BUS_MSB) {
            if (t.get_string() == ":") {
                state = BUS_COLON;
                continue;
            }
            else if (t.get_string() == "]") {
                state = BUS_DONE;
                continue;
            }
        }
        else if (state == BUS_COLON) {
            if (t.get_token_type() == evl_token::NUMBER) {
                pin.set_bus_lsb(atoi(t.get_string().c_str()));
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
            if (t.get_string() == ")") {
                s.get_evl_component_ref().get_pin_vector_ref().push_back(pin);
                state = PINS_DONE;
                continue;
            }
            else if (t.get_string() == ",") {
                s.get_evl_component_ref().get_pin_vector_ref().push_back(pin);
                state = PINS;
                continue;
            }
        }
        else if (state == PINS_DONE) {
            if (t.get_string() == ";") {
                state = DONE;
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
