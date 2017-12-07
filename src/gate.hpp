// gate.hpp
#ifndef GUARD_GATE_H
#define GUARD_GATE_H

class gate {

    std::string name_;

    std::string type_; // e.g. "and", "or"

    char state_;

    char next_state_;

protected:

    std::vector<pin *> pins_; // relationship "contain"

public:

// Constructors

    gate();

    gate(std::string n, std::string t);

    gate(std::string n, std::string t, std::vector<pin *> p, char s_, char ns_);

// Destructors
   
    ~gate();

// Setters

    bool set(std::string n, std::string t, std::vector<pin *> p, char s_, char ns_);

    bool set_name_(std::string n);

    bool set_type_(std::string t);

    bool set_pins_(std::vector<pin *> p);

    bool set_state_(char s_);

    bool set_next_state_(char ns_);

// Getters

    std::string get_name_() const;

    std::string get_type_() const;

    std::vector<pin *> get_pins_() const;

    std::vector<pin *> & get_pins_ref();

    char get_state_() const;

    char get_next_state_() const;

//Other methods

    bool create(const evl_component &c, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table);

    bool create_pin(const evl_pin &ep, size_t index, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table);

    void compute_next_state_or_output(int transition, std::ofstream &file_in, std::ofstream &file_out);

    char compute_signal(int pin_index); 

    bool validate_structural_semantics();

    void update_state();

}; // class gate

#endif
