// gate.hpp
#ifndef GUARD_GATE_H
#define GUARD_GATE_H

class gate {

    std::string name_;

    std::string type_; // e.g. "and", "or"

    std::vector<pin *> pins_; // relationship "contain"

}; // class gate

#endif
