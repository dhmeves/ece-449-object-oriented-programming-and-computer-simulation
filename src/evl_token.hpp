// evl_token.hpp
#ifndef GUARD_TOKEN_H
#define GUARD_TOKEN_H

#include <string>

class evl_token {

public:    

    enum token_type {NAME, NUMBER, SINGLE};
    
private:

    token_type type;
    
    std::string str;
    
    int line_no;

public:

// Constructors
    
    evl_token();

    evl_token(token_type t, std::string s, int l);

// Setters

    bool set(token_type t, std::string s, int l);

    bool set_token_type(token_type t);

    bool set_string(std::string s);

    bool set_line_no(int l);
   
// Getters

    token_type get_token_type() const;

    std::string get_string() const;

    int get_line_no() const;
 
// Other Methods

    static bool is_character_a_comment(char ch);
       
    static bool is_character_a_space(char ch);

    static bool is_character_a_single(char ch); 

    static bool is_character_a_name(char ch);

    static bool is_character_a_number(char ch);

    static bool is_character_a_alpha_num_space_dollar(char ch);

    static bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens);

    static bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens);

    static void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens);

    static bool store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens);

    static bool token_is_semicolon(const evl_token &token);

    static bool has_semicolon(const std::vector<evl_token> &tokens);

}; // class evl_token

#endif
