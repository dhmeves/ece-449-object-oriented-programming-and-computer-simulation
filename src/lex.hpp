// lex.hpp
#ifndef GUARD_LEX_H
#define GUARD_LEX_H

#include <list>
#include <vector>
#include <map>

// Forward Declarations

class evl_token;

class evl_statement;

class evl_wire;

class evl_component;

class evl_pin;

class netlist;

class gate;

class net;

class pin;

class flip_flop;

// Type Definitions

typedef std::list<evl_token> evl_tokens;

typedef std::list<evl_statement> evl_statements;

typedef std::vector<evl_wire> evl_wires;

typedef std::vector<evl_component> evl_components;

typedef std::vector<evl_pin> evl_pins;

typedef std::map<std::string, int> evl_wires_table;

// lex.hpp

class lex {
    
public:

// Constructors

// Destructors

// Setters

// Getters

// Other Methods

    static void remove_all_zeros(std::list<int> &integers);

    static void show_vector(const std::vector<int> &vec);

    static bool parse_evl_file(std::string evl_file, std::string &module_name, evl_wires &wires, evl_components &comps);
};

#endif
