// flip_flop.hpp
#ifndef GUARD_FLIP_FLOP_H
#define GUARD_FLIP_FLOP_H

class flip_flop: public gate {
    char state_, next_state_;
    public:
    bool validate_structural_semantics();
    void compute_next_state_or_output();
    char compute_signal(int pin_index);
}; // class flip_flop 

#endif
