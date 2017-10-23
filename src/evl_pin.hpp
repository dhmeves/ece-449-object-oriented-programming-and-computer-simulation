// evl_pin.hpp
#ifndef GUARD_EVL_PIN_H
#define GUARD_EVL_PIN_H

class evl_pin {
    
    std::string name;

    int bus_msb;

    int bus_lsb;

public:

// Constructors

    evl_pin();

    evl_pin(std::string n, int msb, int lsb);

// Setters

    bool set(std::string n, int msb, int lsb);

    bool set_name(std::string n);

    bool set_bus_msb(int msb);

    bool set_bus_lsb(int lsb);

// Getters

    std::string get_name() const;

    int get_bus_msb() const;

    int get_bus_lsb() const;

// Other Methods    

};

#endif
