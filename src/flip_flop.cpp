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
#include "flip_flop.hpp"
//#include "Vec.hpp"


bool flip_flop::validate_structural_semantics() {
    if (pins_.size() != 3) return false;
    pins_[0]->set_as_output(); // q
    pins_[1]->set_as_input(); // d
    pins_[2]->set_as_input(); // clk
}
void flip_flop::compute_next_state_or_output() {
    next_state_ = pins_[1]->compute_signal(); // d
}

char flip_flop::compute_signal(int pin_index) {
    assert(pin_index == 0); // must be q
    return state_;
}
