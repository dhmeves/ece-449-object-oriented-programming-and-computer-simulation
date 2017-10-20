// evl_statement.hpp
#ifndef GUARD_STATEMENT_H
#define GUARD_STATEMENT_H

#include "evl_component.hpp"

class evl_statement {

public:

    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};
    
private:

    statement_type type;
    
    evl_tokens tokens;

    evl_wires_table wires_table;

    evl_wires wires_vector;

    evl_component component;

public:

// Constructors
        
    evl_statement();

    evl_statement(statement_type t, evl_tokens tok, evl_wires_table wire_tab, evl_wires wv, evl_component comp);

// Setters

    bool set(statement_type t, evl_tokens tok, evl_wires_table wire_tab, evl_wires wv, evl_component comp);

    bool set_statement_type(statement_type t);

    bool set_evl_tokens(evl_tokens tok);

    bool set_evl_wires_table(evl_wires_table wire_tab);

    bool set_evl_wires_vector(evl_wires wv);

    bool set_evl_component(evl_component comp);

// Getters

    statement_type get_statement_type() const;

    evl_tokens get_evl_tokens() const;

    evl_tokens & get_evl_tokens_ref();

    evl_wires_table get_evl_wires_table() const;

    evl_wires_table & get_evl_wires_table_ref();

    evl_wires get_evl_wires_vector() const;

    evl_wires & get_evl_wires_vector_ref();

    evl_component get_evl_component() const;

    evl_component & get_evl_component_ref();

// Other methods

    static bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

    static bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

    static void display_statements(std::ostream &out, std::vector<evl_statement> &statements); 

    static bool store_statements_to_file(std::string file_name, std::vector<evl_statement> &statements); 

}; // class evl_statement

#endif
