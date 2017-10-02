#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <algorithm>

bool is_character_a_comment(char ch);
   
bool is_character_a_space(char ch);

bool is_character_a_single(char ch); 

bool is_character_a_name(char ch);

bool is_character_a_number(char ch);

bool is_character_a_alpha_num_space_dollar(char ch);

struct evl_token;
     
bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens);

bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens);

void display_tokens(std::vector<evl_token> tokens);

bool store_tokens_to_file(std::string file_name, std::vector<evl_token> tokens);

typedef std::list<evl_token> evl_tokens;

struct evl_statement;

typedef std::list<evl_statement> evl_statements;

bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

struct evl_wire;

typedef std::vector<evl_wire> evl_wires;

bool process_wire_statement(evl_wires &wires, evl_statement &s);
