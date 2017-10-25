// pin.hpp
#ifndef GUARD_PIN_H
#define GUARD_PIN_H

class pin {

    char dir_; // e.g. ’I’ or ’O’

    gate *gate_; // relationship "contain"

    size_t index_; // attribute of "contain"

    net *net_; // relationship "connect"

public:

// Constructors

    pin();

    pin(gate *g, size_t i);

    pin(char d, gate *g, size_t i, net *n);

// Destructors

// Setters

    bool set(char d, gate *g, size_t i, net *n);

    bool set_dir_(char d);

    bool set_gate_ptr(gate *g);

    bool set_index_(size_t i);

    bool set_net_ptr(net *n);

    bool set_as_input();

    bool set_as_output();

// Getters

    char get_dir_() const;

    gate * get_const_gate_ptr() const;

    gate * get_gate_ptr();

    size_t get_index_() const;

    net * get_const_net_ptr() const;

    net * get_net_ptr();

// Other Methods

    bool create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net *> &nets_table);

    char compute_signal();

}; // class pin

#endif
