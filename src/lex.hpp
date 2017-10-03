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

struct evl_token;
     
typedef std::list<evl_token> evl_tokens;

bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens);

bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens);

void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens);

bool store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens);

struct evl_statement;

typedef std::list<evl_statement> evl_statements;

bool token_is_semicolon(const evl_token &token);

bool has_semicolon(const evl_tokens &tokens);

void remove_all_zeros(std::list<int> &integers);

void show_vector(const std::vector<int> &vec);

bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

struct evl_wire;

typedef std::vector<evl_wire> evl_wires;

bool process_wire_statement(evl_wires &wires, evl_statement &s);

bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
 
void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);  
