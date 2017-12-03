// netlist.hpp
#ifndef GUARD_NETLIST_H
#define GUARD_NETLIST_H

class netlist {

    std::list<gate *> gates_;

    std::list<net *> nets_;

    std::map<std::string, net *> nets_table_;

public:

// Constructors
    
    netlist();

    netlist(std::list<gate *> g, std::list<net *> n, std::map<std::string, net *> nt);

// Destructors
    
    ~netlist();

// Setters
    
    bool set(std::list<gate *> g, std::list<net *> n, std::map<std::string, net *> nt);
        
    bool set_gates_(std::list<gate *> g);

    bool set_nets_(std::list<net *> n);

    bool set_nets_table_(std::map<std::string, net *> nt);

// Getters

    std::list<gate *> get_gates_() const;

    std::list<gate *> & get_gates_ref();

    std::list<net *> get_nets_() const;

    std::list<net *> & get_nets_ref();

    std::map<std::string, net *> get_nets_table_() const;

    std::map<std::string, net *> & get_nets_table_ref();

// Other Methods

    bool create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table);

    bool create_nets(const evl_wires &wires);

    std::string make_net_name(std::string wire_name, int i);

    void create_net(std::string net_name);

    bool create_gates(const evl_components &comps, const evl_wires_table &wires_table);

    bool create_gate(const evl_component &c, const evl_wires_table &wires_table);

    bool save(std::string nl_fl, std::string mod_name);

    void compute_next_state_and_output(int time, std::string file_name);
    
    void simulate(int time, std::string file_name);

}; // class netlist

#endif
