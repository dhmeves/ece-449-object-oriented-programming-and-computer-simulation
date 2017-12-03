// netlist.cpp

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

netlist::netlist() {}

netlist::netlist(std::list<gate *> g, std::list<net *> n, std::map<std::string, net *> nt) : gates_(g), nets_(n), nets_table_(nt) {}

// Destructors

netlist::~netlist() {
    // ... // delete pointers stored in gates_ and nets_
}

// Setters

bool netlist::set(std::list<gate *> g, std::list<net *> n, std::map<std::string, net *> nt) {
    //...// return false if netlist is invalid
    gates_ = g;
    nets_ = n;
    nets_table_ = nt;
    return true;
}

bool netlist::set_gates_(std::list<gate *> g) {
    //...// return false if gates are invalid
    gates_ = g;
    return true;
}

bool netlist::set_nets_(std::list<net *> n) {
    //...// return false if nets are invalid
    nets_ = n;
    return true;
}

bool netlist::set_nets_table_(std::map<std::string, net *> nt) {
    //...// return false if nets_table_ is invalid
    nets_table_ = nt;
    return true;   
}

// Getters

std::list<gate *> netlist::get_gates_() const {
    return gates_;
}

std::list<gate *> & netlist::get_gates_ref() {
    return gates_;
}

std::list<net *> netlist::get_nets_() const {
    return nets_;
}

std::list<net *> & netlist::get_nets_ref() {
    return nets_;
}

std::map<std::string, net *> netlist::get_nets_table_() const {
    return nets_table_;
}

std::map<std::string, net *> & netlist::get_nets_table_ref() {
    return nets_table_;
}

// Other Methods

bool netlist::create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table) {
    return create_nets(wires) && create_gates(comps, wires_table);
}

bool netlist::create_nets(const evl_wires &wires) {
    for (auto &w : wires) {
        if (w.get_width() == 1) {
            create_net(w.get_name());
        }
        else {
            for (int i = 0; i < w.get_width(); ++i) {
                create_net(make_net_name(w.get_name(), i));
            }
        }
    }
    return true;
}

std::string netlist::make_net_name(std::string wire_name, int i) {
    assert(i >= 0);
    std::ostringstream oss;
    oss << wire_name << "[" << i << "]";
    return oss.str();
}  

void netlist::create_net(std::string net_name) {
    assert(nets_table_.find(net_name) == nets_table_.end());
    net *n = new net(net_name);
    nets_table_[net_name] = n;
    nets_.push_back(n);
}

bool netlist::create_gates(const evl_components &comps, const evl_wires_table &wires_table) {
    for (auto &c : comps) {
        create_gate(c, wires_table);
    }
    return true;
}

bool netlist::create_gate(const evl_component &c, const evl_wires_table &wires_table) {
    gate *g = new gate;
    gates_.push_back(g);
    return g->create(c, nets_table_, wires_table);
}

bool netlist::save(std::string nl_fl, std::string mod_name) {
    std::ofstream output_file(nl_fl.c_str());
    //... // verify output_file is ready
    if (!output_file) {
        std::cerr << "I can't write " << nl_fl << std::endl;
        return false;
    }
    output_file << "module " << mod_name << std::endl;
//    std::vector<net *> net_ptr_vec{std::begin(nets_), std::end(nets_)};
//    std::vector<gate *> gate_ptr_vec{std::begin(gates_), std::end(gates_)};
    output_file << "nets " << nets_.size() << std::endl;
    for (auto n : nets_) {
        output_file << "  " << "net " << n->get_name_() << " " << n->get_connections_ref().size() << std::endl;
        std::vector<pin *> pin_ptr_vec{std::begin(n->get_connections_ref()), std::end(n->get_connections_ref())};
        for (auto g : gates_) {
            for (int i = 0; i < n->get_connections_ref().size(); ++i) {
                if (g == pin_ptr_vec[i]->get_gate_ptr()) {
                    output_file << "    " << g->get_type_();
                    if (g->get_name_() != "") 
                        output_file << g->get_name_();
                    output_file << " " << pin_ptr_vec[i]->get_index_() << std::endl;
                }
            }
        }
    }
    output_file << "components " << gates_.size() << std::endl;
    for (auto g : gates_) {
        output_file << "  " << "component " << g->get_type_();
        if (g->get_name_() != "")
            output_file << g->get_name_();
        output_file << " " << g->get_pins_().size() << std::endl;
        for (auto p : g->get_pins_()) {
            output_file << "    " << "pin " << p->get_width_();
            for (int k = 0; k < p->get_net_ptr().size() ; ++k) {
                output_file << " " << p->get_net_ptr()[k]->get_name_();  
            }  
            output_file << std::endl;
        }
    }
    return true;
}

// project 4

void netlist::compute_next_state_and_output(int time, std::string file_name) {
    for (net *n: nets_)
        n->set_signal_('?');
    for (gate *g: gates_)
        g->compute_next_state_or_output(time, file_name);
}

void netlist::simulate(int time, std::string file_name) {
    compute_next_state_and_output(time, file_name);
}
