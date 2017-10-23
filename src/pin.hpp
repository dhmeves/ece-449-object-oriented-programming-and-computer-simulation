// pin.hpp
#ifndef GUARD_PIN_H
#define GUARD_PIN_H

class pin {

    char dir_; // e.g. ’I’ or ’O’

    gate *gate_; // relationship "contain"

    size_t index_; // attribute of "contain"

    net *net_; // relationship "connect"

}; // class pin

#endif
