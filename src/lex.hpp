#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <map>

bool is_character_a_comment(char ch);
   
bool is_character_a_space(char ch);

bool is_character_a_single(char ch); 

bool is_character_a_name(char ch);

bool is_character_a_number(char ch);

bool is_character_a_alpha_num_space_dollar(char ch);

typedef std::list<evl_token> evl_tokens;

typedef std::list<evl_statement> evl_statements;

bool token_is_semicolon(const evl_token &token);

bool has_semicolon(const evl_tokens &tokens);

void remove_all_zeros(std::list<int> &integers);

void show_vector(const std::vector<int> &vec);

typedef std::vector<evl_wire> evl_wires;

bool process_wire_statement(evl_wires &wires, evl_statement &s);

bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
 
void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);  
