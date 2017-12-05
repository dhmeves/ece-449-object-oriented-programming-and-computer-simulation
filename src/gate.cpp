// gate.cpp

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
#include <sstream>
#include <iomanip>
#include <unistd.h>

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

gate::gate() {}

gate::gate(std::string n, std::string t) : name_(n), type_(t) {}

gate::gate(std::string n, std::string t, std::vector<pin *> p, char s_, char ns_) : name_(n), type_(t), pins_(p), state_(s_), next_state_(ns_) {}

// Destructors

gate::~gate() {
    for (size_t i = 0; i < pins_.size(); ++i)
        delete pins_[i];
}

// Setters

bool gate::set(std::string n, std::string t, std::vector<pin *> p, char s_, char ns_) {
    //,..// return false if gate is invalid
    name_ = n;
    type_ = t;
    pins_ = p;
    state_ = s_;
    next_state_ = ns_;
    return true;
}

bool gate::set_name_(std::string n){
    //...// return false if name is invalid
    name_ = n;
    return true;
}

bool gate::set_type_(std::string t){
    //...// return false if type is invalid
    type_ = t;
    return true;
}

bool gate::set_pins_(std::vector<pin *> p) {
    //...// return false if pins are invalid
    pins_ = p;
    return true;
}

bool gate::set_state_(char s_) {
    //...// return false if state is invalid
    state_ = s_;
    return true;
}

bool gate::set_next_state_(char ns_) {
    //...// return false if next state is invalid
    next_state_ = ns_;
    return true;
}

// Getters

std::string gate::get_name_() const {
    return name_;
}

std::string gate::get_type_() const {
    return type_;
}

std::vector<pin *> gate::get_pins_() const {
    return pins_;
}

std::vector<pin *> & gate::get_pins_ref() {
    return pins_;
}

char gate::get_state_() const {
    return state_;
}

char gate::get_next_state_() const {
    return next_state_;
}

// Other Methods

bool gate::create(const evl_component &c, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table) {
    // set gate type and name;
    name_ = c.get_name();
    type_ = c.get_type();
    size_t index = 0;
    state_ = '0';
    for (auto &ep : c.get_pin_vector()) {
        create_pin(ep, index, nets_table, wires_table);
        ++index;
    }
//    return true;
    return validate_structural_semantics();
}

bool gate::create_pin(const evl_pin &ep, size_t index, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table) {
    // resolve semantics of ep using wires_table
    pin *p = new pin;
    auto wire = wires_table.find(ep.get_name());
    int width = wire->second;
    pins_.push_back(p);
    return p->create(this, index, ep, nets_table, width);
}

// project 4

void gate::compute_next_state_or_output(std::ofstream &file_out) {
    if (type_ == "evl_dff") {
        next_state_ = pins_[1]->compute_signal(); // d
    }
    else if (type_ == "evl_output") {
        //collect signal from all pins and write to file
        for (auto pin : pins_) {
            std::string temp;
            for (auto n : pin->get_net_ptr()) {
                temp = temp+n->get_signal();
            }
            int result = 0;
            for (size_t count = 0; count < temp.length() ; ++count) {
                result *=2;
                result += temp[count]=='1'? 1 :0;
            }  
            int temp2 = pin->get_width_();  
            int rounded_pin_width = temp2/4;
            rounded_pin_width += temp2 % 4 ? 1 : 0;
            std::stringstream ss;
            ss << std::hex << std::setw(rounded_pin_width) << std::setfill('0')  << result;
            file_out << ss.str() << " ";
        }
        file_out << std::endl;
    }
}

char gate::compute_signal(int pin_index) {
    if (type_ == "evl_dff") {
        assert(pin_index == 0); // must be q
        return state_;
    }
    else if (type_ == "evl_zero") {
        return '0';
    }
    else if (type_ == "and") {
        assert(pin_index == 0); // must be out
//        collect signals from the input pins
//        compute and return the output signal
        for (auto p : pins_) {
            if (p->get_dir_() == 'I') {
                for (auto n : p->get_net_ptr()) {
                    if (n->get_signal() == '0') {
                        return '0';
                    }
                    else
                        continue;
                }
            }
            else
                continue;
        }
        return '1';    
    }
    //...//
    else if (type_ == "or") {
        for (auto p : pins_) {
            if (p->get_dir_() == 'I') {
                for (auto n: p->get_net_ptr()) {
                    if (n->get_signal() == '1') {
                        return '1';
                    }
                    else
                        continue;
                }
            }
        }
        return '0'; 
    }
    else if (type_ == "xor") {
        int flag = 0;
        for (auto p : pins_) {
            if (p->get_dir_() == 'I') {
                for (auto n : p->get_net_ptr()) {
                    if (n->get_signal() == '1') {
                        flag++;
                    }   
                    else 
                        continue;
                }
            }
        }
        if ((flag % 2) == 0) {
            return '0';
        }
        else
            return '1';
    }
    else if (type_ == "not") {
        for (auto p : pins_) {
            if (p->get_dir_() == 'I') {
                for (auto n : p->get_net_ptr()) {
                    if (n->get_signal() == '1') {
                        return '0';
                    }   
                    else if (n->get_signal() == '0') {
                        return '1';
                    }
                    else
                        continue;
                }
            }
        }
    }
    else if (type_ == "buf") {
        for (auto p : pins_) {
            if (p->get_dir_() == 'I') {
                for (auto n : p->get_net_ptr()) {
                    if (n->get_signal() == '1') {
                        return '1';
                    }   
                    else if (n->get_signal() == '0') {
                        return '0';
                    }
                    else
                        continue;
                }
            }
        }
    }
    else if (type_ == "evl_one") {
        return '1';
    }
}

bool gate::validate_structural_semantics() {
    if (type_ == "and") {
        if (pins_.size() < 3) return false;
        pins_[0]->set_as_output(); // out
        for (size_t i = 1; i < pins_.size(); ++i)
            pins_[i]->set_as_input();
    }
    else if (type_ == "or") {
        if (pins_.size() < 3) return false;
        pins_[0]->set_as_output(); // out
        for (size_t i = 1; i < pins_.size(); ++i)
            pins_[i]->set_as_input();
    }
    else if (type_ == "xor") {
        if (pins_.size() < 3) return false;
        pins_[0]->set_as_output(); // out
        for (size_t i = 1; i < pins_.size(); ++i)
            pins_[i]->set_as_input(); 
    }
    else if (type_ == "not") {
        if (pins_.size() != 2) return false;
        pins_[0]->set_as_output();
        pins_[1]->set_as_input();
    }
    else if (type_ == "buf") {
        if (pins_.size() != 2) return false;
        pins_[0]->set_as_output();
        pins_[1]->set_as_input();
    }
    else if (type_ == "tris") {
        if (pins_.size() != 2) return false;
        pins_[0]->set_as_output();
        pins_[1]->set_as_input();
    }
    else if (type_ == "evl_clock") {
        if (pins_.size() != 1) return false;
        pins_[0]->set_as_output();
    }
    else if (type_ == "evl_dff") {
        if (pins_.size() != 3) return false;
        pins_[0]->set_as_output(); // q
        pins_[1]->set_as_input(); // d
        pins_[2]->set_as_input(); // clk
    }
    else if (type_ == "evl_one") {
        if (pins_.size() < 1) return false;
        for (size_t i = 0; i < pins_.size(); ++i)
            pins_[i]->set_as_output();  
    }
    else if (type_ == "evl_zero") {
        if (pins_.size() < 1) return false;
        for (size_t i = 0; i < pins_.size(); ++i)
            pins_[i]->set_as_output();
    }
    else if (type_ == "evl_input") {
        if (pins_.size() < 1) return false;
        for (size_t i = 0; i < pins_.size(); ++i)
            pins_[i]->set_as_output();
    }
    else if (type_ == "evl_output") {
        if (pins_.size() < 1) return false;
        for (size_t i = 0; i < pins_.size(); ++i)
            pins_[i]->set_as_input(); 
    }
    return true;
}

void gate::update_state() {
    if (type_ == "evl_dff") {
        state_ = next_state_;
    }
}
