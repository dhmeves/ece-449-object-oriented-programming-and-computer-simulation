// net.hpp
#ifndef GUARD_NET_H
#define GUARD_NET_H

class net {
    
    std::string name_; // e.g. "a", "s[0]"
    
    char signal_; // e.g. ’0’ or ’1’
    
    std::list<pin *> connections_; // relationship "connect"

public:

// Constructors

    net();

    net(std::string n, char s, std::list<pin *> c);     

// Destructors

// Setters

    bool set(std::string n, char s, std::list<pin *> c);

    bool set_name_(std:string n);

    bool set_signal_(char s);

    bool set_connections_(std::list<pin *> c);

// Getters

    std::string get_name_() const;

    char get_signal_() const;

    std::list<pin *> get_connections_() const;

    std::list<pin *> & get_connections_ref();

// Other Methods

    static void append_pin(pin *p);

    static char get_signal();

}; // class net

#endif
