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

evl_component::evl_component() {}

evl_component::evl_component(evl_token::token_type t, std::string n, evl_pins pv) : type(t), name(n), pin_vector(pv) {}

bool evl_component::set(evl_token::token_type t, std::string n, evl_pins pv) {
    //...// return false if component is not valid
    type = t;
    name = n;
    pin_vector = pv;
    return true;
}

bool evl_component::set_type(evl_token::token_type t) {
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

evl_token::token_type evl_component::get_type() const {
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

bool evl_component::process_component_statement(evl_components &components, evl_statement &s) {
    //...
    enum state_type {INIT, TYPE, DONE, NAME, PINS, PIN_NAME, PINS_DONE, BUS, BUS_MSB, BUS_COLON, BUS_LSB, BUS_DONE};
    state_type state = INIT;
    for (; !s.get_evl_tokens_ref().empty() && (state != DONE); s.get_evl_tokens_ref().pop_front()) {
        evl_token t = s.get_evl_tokens_ref().front();
        evl_component component;
        evl_pin pin;
        evl_pins pin_vec;
        //... // use branches here to compute state transitions
        if (state == INIT) {
            //...
            if (t.get_token_type() == evl_token::NAME) {
                component.set(t.get_token_type(), "", pin_vec);            
                state = TYPE;
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
            }
            if (t.get_token_type() == evl_token::NAME) {
                component.set_name(t.get_string());
                state = NAME;
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
            }
            else if (t.get_string() == ")") {
                component.get_pin_vector_ref().push_back(pin);                
                state = PINS_DONE;
            }
            else if (t.get_string() == ",") {
                component.get_pin_vector_ref().push_back(pin);
                state = PINS;
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
            }
            else if (t.get_string() == "]") {
                state = BUS_DONE;
            }
        }
        else if (state == BUS_COLON) {
            if (t.get_token_type() == evl_token::NUMBER) {
                pin.set_bus_lsb(atoi(t.get_string().c_str()));
                state == BUS_LSB;
            }
        }
        else if (state == BUS_LSB) {
            if (t.get_string() == "]") {
                state == BUS_DONE;
            }
        }
        else if (state == BUS_DONE) {
            if (t.get_string() == ")") {
                component.get_pin_vector_ref().push_back(pin);
                state = PINS_DONE;
            }
            else if (t.get_string() == ",") {
                component.get_pin_vector_ref().push_back(pin);
                state = PINS;
            }
        }
        else if (state == PINS_DONE) {
            if (t.get_string() == ";") {
                state = DONE;
            }
        }
    }
    if (!s.get_evl_tokens().empty() || (state != DONE)) {
        std::cerr << "something wrong with the statement" << std::endl;
        return false;
    }
    return true;
}