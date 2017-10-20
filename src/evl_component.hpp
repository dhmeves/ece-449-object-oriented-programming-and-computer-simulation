// evl_component.hpp
#ifndef GUARD_COMPONENT_H
#define GUARD_COMPONENT_H

class evl_component {

    std::string type;    

    std::string name;
    
    evl_pins pin_vector;    
    
public:    

// Constructors

    evl_component();

    evl_component(std::string t, std::string n, evl_pins pv);

// Setters

    bool set(std::string t, std::string n, evl_pins pv);

    bool set_type(std::string t);

    bool set_name(std::string n);

    bool set_pin_vector(evl_pins pv);

// Getters

    std::string get_type() const;

    std::string get_name() const;

    evl_pins get_pin_vector() const;

    evl_pins & get_pin_vector_ref();

// Other Methods

    static bool process_component_statement(evl_statement &s);

};

#endif
