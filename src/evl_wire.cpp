#include "evl_wire.hpp"

class evl_wire {

    std::string name;
    
    int width;

public:
    
    evl_wire();

    bool process_wire_statement(evl_wires &wires, evl_statement &s) {
        //...
        enum state_type {INIT, WIRE, DONE, WIRES, WIRE_NAME};
        state_type state = INIT;
        for (; !s.tokens.empty() && (state != DONE); s.tokens.pop_front()) {
            evl_token t = s.tokens.front();
            //... // use branches here to compute state transitions
            if (state == INIT) {
                //...
                if (t.str == "wire") {
                    state = WIRE;
                }
                else {
                    std::cerr << "Need ’wire’ but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
            else if (state == WIRE) {
                //...
                if (t.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = t.str; wire.width = 1;
                    wires.push_back(wire);
                    state = WIRE_NAME;
                }
                else {
                    std::cerr << "Need NAME but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
            else if (state == WIRES) {
                //...
                //... // same as the branch for WIRE
            }
            else if (state == WIRE_NAME) {
                //...
                if (t.str == ",") {
                    state = WIRES;
                }
                else if (t.str == ";") {
                    state = DONE;
                }
                else {
                    std::cerr << "Need ’,’ or ’;’ but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
        }
        if (!s.tokens.empty() || (state != DONE)) {
            std::cerr << "something wrong with the statement" << std::endl;
            return false;
        }
        return true;
    }

    bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table) {
        for (auto &wire: wires) {
            auto same_name = wires_table.find(wire.name);
            if (same_name != wires_table.end()) {
                std:cerr << "Wire '" << wire.name
                    << "'is already defined" << std::end1;
                return false;
            }
            //wires_table[wire.name] = wire.width;
            wires_table.insert(std::make_pair(wire.name, wire.width));
        }
        return true;
    }

    void display_wires_table(std::ostream &out, const evl_wires_table &wires_table) {
        for (evl_wires_table::const_iterator it = wires_table.begin();
            it != wires_table.end(); ++it) {
            out << "wire " << it->first
                << " " << it->second << std::endl;
        }
    }

} // class evl_wire
