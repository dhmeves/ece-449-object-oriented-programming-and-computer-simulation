// pin.hpp
#ifndef GUARD_PIN_H
#define GUARD_PIN_H

class pin {

    char dir_; // e.g. ’I’ or ’O’

    gate *gate_; // relationship "contain"

    size_t index_; // attribute of "contain"

    std::vector<net *> nets_; // relationship "connect"
    
//    net *net_; // relationship "connect"

    int width_;

public:

// Constructors

    pin();

    pin(gate *g, size_t i);

    pin(char d, gate *g, size_t i, std::vector<net *> n, int w);

// Destructors

// Setters

    bool set(char d, gate *g, size_t i, std::vector<net *> n, int w);

    bool set_dir_(char d);

    bool set_gate_ptr(gate *g);

    bool set_index_(size_t i);
    
    bool set_nets_(std::vector<net *> n);

//    bool set_net_ptr(net *n);

    bool set_width_(int w);

    bool set_as_input();

    bool set_as_output();

// Getters

    char get_dir_() const;

    gate * get_const_gate_ptr() const;

    gate * get_gate_ptr();

    size_t get_index_() const;

    std::vector<net *> get_const_net_ptr() const;

    std::vector<net *> get_net_ptr();

//    net * get_const_net_ptr() const;

//    net * get_net_ptr();

    int get_width_() const;

// Other Methods

    bool calculate_width(const evl_pin &p, int width);

    bool create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net *> &nets_table, int width);

    char compute_signal();

}; // class pin

#endif
