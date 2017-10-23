// evl_statement.cpp
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
#include "Vec.hpp"

// Constructors

evl_statement::evl_statement() {}

evl_statement::evl_statement(statement_type t, evl_tokens tok, evl_wires_table wire_tab, evl_wires wv, evl_component comp) 
    : type(t), tokens(tok), wires_table(wire_tab), wires_vector(wv), component(comp) {}

// Destructors

// Setters

bool evl_statement::set(statement_type t, evl_tokens tok, evl_wires_table wire_tab, evl_wires wv, evl_component comp) {
    //,,,// return false if statement is not valid
    type = t;
    tokens = tok;
    wires_table = wire_tab;
    wires_vector = wv;
    component = comp;
    return true;
}

bool evl_statement::set_statement_type(statement_type t) {
    //...// return false if statment type is invalid
    type = t;
    return true;
}

bool evl_statement::set_evl_tokens(evl_tokens tok) {
    //...// return false if tokens are invalid
    tokens = tok;
    return true;
}

bool evl_statement::set_evl_wires_table(evl_wires_table wire_tab) {
    //...// return false if wires table is invalid
    wires_table = wire_tab;
    return true;
}

bool evl_statement::set_evl_wires_vector(evl_wires wv) {
    //...// return false if wires vector is invalid
    wires_vector = wv;
    return true;
}

bool evl_statement::set_evl_component(evl_component comp) {
    //...// return false if component is invalid
    component = comp;
    return true;
}

// Getters

evl_statement::statement_type evl_statement::get_statement_type() const{
    return type;
}

evl_tokens evl_statement::get_evl_tokens() const{
    return tokens;
}

evl_tokens & evl_statement::get_evl_tokens_ref() {
    return tokens;
}

evl_wires_table evl_statement::get_evl_wires_table() const{
    return wires_table;
}

evl_wires_table & evl_statement::get_evl_wires_table_ref() {
    return wires_table;
}

evl_wires evl_statement::get_evl_wires_vector() const{
    return wires_vector;
}

evl_wires & evl_statement::get_evl_wires_vector_ref() {
    return wires_vector;
}

evl_component evl_statement::get_evl_component() const{
    return component;
}

evl_component & evl_statement::get_evl_component_ref() {
    return component;
}

// Other Methods

bool evl_statement::group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens) {
    for (; !tokens.empty();) { // generate one statement per iteration
        evl_token token = tokens.front();
        if (token.get_token_type() != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << token.get_string()
                << "' on line " << token.get_line_no() << std::endl;
            return false;
        }
        else if (token.get_string() == "module") { // MODULE statement
            //...
            evl_tokens token_list_mod;
            evl_statement module;
            evl_wires_table wire_tab_mod;
            evl_wires wire_vec_mod;
            evl_component comp1; 
            module.set(evl_statement::MODULE, token_list_mod, wire_tab_mod, wire_vec_mod, comp1);
            tokens.pop_front();
            // Thinking of a function to replace the loop?
            if (!move_tokens_to_statement(module.get_evl_tokens_ref(), tokens))
                return false;
            /* for (; !tokens.empty();) {
                module.get_evl_tokens().push_back(tokens.front());
                tokens.pop_front(); // consume one token per iteration
                if (module.get_evl_tokens().back().get_string() == ";")
                    break; // exit if the ending ";" is found
            }
            if (module.get_evl_tokens().back().get_string() != ";") {
                std::cerr << "Look for ’;’ but reach the end of file" << std::endl;
                return false;
            }*/

            statements.push_back(module);
            continue;
        }   
        else if (token.get_string() == "endmodule") { // ENDMODULE statement
            //...
            evl_tokens token_list_endmod;
            evl_statement endmodule;
            evl_wires_table wire_tab_endmod;
            evl_wires wire_vec_endmod;
            evl_component comp2;
            endmodule.set(evl_statement::ENDMODULE, token_list_endmod, wire_tab_endmod, wire_vec_endmod, comp2);
            endmodule.get_evl_tokens_ref().push_back(token);
            tokens.pop_front();     // consume last token
            statements.push_back(endmodule);
            continue;
        }
        else if (token.get_string() == "wire") { // WIRE statement
            //...
            evl_tokens token_list_wir;
            evl_statement wire; 
            evl_wires_table wire_tab_wir;
            evl_wires wire_vec_wir;
            evl_component comp3;
            wire.set(evl_statement::WIRE, token_list_wir, wire_tab_wir, wire_vec_wir, comp3);
            evl_wires wire_vec;
            if (!move_tokens_to_statement(wire.get_evl_tokens_ref(), tokens))
                return false;
            if (!evl_wire::process_wire_statement(wire_vec, wire)) 
                return false;
            if (!evl_wire::make_wires_table(wire_vec, wire.get_evl_wires_table_ref())) 
                return false;
            if (!evl_wire::make_wires_vector(wire_vec, wire.get_evl_wires_vector_ref()))
                return false;
            statements.push_back(wire);
            //evl_wire::display_wires_table(std::cout, wire.get_evl_wires_table());
            continue; 
        }
        else { // COMPONENT statement
            //...
            evl_tokens token_list_comp;
            evl_statement component;
            evl_wires_table wire_tab_comp;
            evl_wires wire_vec_comp;
            evl_component comp0;
            component.set(evl_statement::COMPONENT, token_list_comp, wire_tab_comp, wire_vec_comp, comp0);
            if (!move_tokens_to_statement(component.get_evl_tokens_ref(), tokens)) 
                return false;
            if (!evl_component::process_component_statement(component))
                return false;
            statements.push_back(component);
            continue;
        }
    }
    return true;
}

bool evl_statement::move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens) {
    // What if someone passes the two parameters in the wrong order?
    assert(statement_tokens.empty());
    // search for ";"
    evl_tokens::iterator next_sc = std::find_if(
        tokens.begin(), tokens.end(), evl_token::token_is_semicolon);
    if (next_sc == tokens.end()) {
        std::cerr << "Look for ’;’ but reach the end of file" << std::endl;
        return false;
    }
    // move tokens within [tokens.begin(), next_sc] to statement_tokens
    evl_tokens::iterator after_sc = next_sc; ++after_sc;
    statement_tokens.splice(statement_tokens.begin(),
        tokens, tokens.begin(), after_sc);
    return true;
}

void evl_statement::display_statements(std::ostream &out, std::vector<evl_statement> &statements) {
    int wire_count = 0;    
    int component_count = 0;
    for (size_t i = 0; i < statements.size(); ++i) {
        if (statements[i].get_statement_type() == evl_statement::MODULE) {
            out << "module " << statements[i].get_evl_tokens().front().get_string() << std::endl;
            continue;
        }
        else if (statements[i].get_statement_type() == evl_statement::WIRE) {
            int temp = statements[i].get_evl_wires_vector().size();
            wire_count += temp;
            continue;
        }
        else if (statements[i].get_statement_type() == evl_statement::COMPONENT) {
            component_count++;
            continue;
        }
    }
    out << "wires " << wire_count << std::endl;
    for (size_t l = 0; l < statements.size(); ++l) {
        evl_wire::display_wires_vector(out, statements[l].get_evl_wires_vector());
    }  
    out << "components " << component_count << std::endl;
    for (size_t j = 0; j < statements.size(); ++j) {
        if (statements[j].get_statement_type() == evl_statement::COMPONENT) {
            out << "component " << statements[j].get_evl_component_ref().get_type() 
                << statements[j].get_evl_component_ref().get_name() << " " 
                << statements[j].get_evl_component_ref().get_pin_vector_ref().size() << std::endl;
            for (size_t k = 0; k < statements[j].get_evl_component_ref().get_pin_vector_ref().size(); ++k) {
                if ((statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_msb() != -1) &&
                    (statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_lsb() != -1)) {
                    out << "pin " << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_name() << " "
                        << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_msb() << " "
                        << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_lsb() << std::endl;
                }
                else if ((statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_msb() != -1) &&
                         (statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_lsb() == -1)) {
                    out << "pin " << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_name() << " "
                        << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_msb() << std::endl;
                }
                else if ((statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_msb() == -1) &&
                         (statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_bus_lsb() == -1)) {
                    out << "pin " << statements[j].get_evl_component_ref().get_pin_vector_ref()[k].get_name() << std::endl;
                }
            }
            continue;
        }        
    }
}   

bool evl_statement::store_statements_to_file(std::string file_name, std::vector<evl_statement> &statements) {
    std::ofstream output_file(file_name.c_str());
    //... // verify output_file is ready
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << ".syntax ." << std::endl;
        return -1;
    }
    // almost the same loop as display_tokens
    display_statements(output_file, statements);
    return true;
}
