// evl_token.cpp
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

evl_token::evl_token() {}

evl_token::evl_token(token_type t, std::string s, int l) : type(t), str(s), line_no(l) {}

bool evl_token::set(token_type t, std::string s, int l) {
    //...// return false if token is not valid
    type = t;
    str = s;
    line_no = l;
    return true;
}

bool evl_token::set_token_type(token_type t) {
    //...// return false if type is invalid
    type = t;
    return true;
}

bool evl_token::set_string(std::string s) {
    //...// return false if string is invalid
    str = s;
    return true;
}

bool evl_token::set_line_no(int l) {
    //...// return false if line number is invalid
    line_no = l;
    return true;
}  

evl_token::token_type evl_token::get_token_type() const {
    return type;
} 

std::string evl_token::get_string() const {
    return str;
}

int evl_token::get_line_no() const {
    return line_no;
} 

bool evl_token::is_character_a_comment(char ch) {
    return (ch == '/');
}    

bool evl_token::is_character_a_space(char ch) {
    return (ch == ' ') || (ch == '\t')
        || (ch == '\r') || (ch == '\n');
}

bool evl_token::is_character_a_single(char ch) {
    return (ch == '(') || (ch == ')')
        || (ch == '[') || (ch == ']')
        || (ch == ':') || (ch == ';')
        || (ch == ','); 
}

bool evl_token::is_character_a_name(char ch) {
    return ((ch >= 'a') && (ch <= 'z'))    // a to z
        || ((ch >= 'A') && (ch <= 'Z'))    // A to Z
        || (ch == '_'); 
}

bool evl_token::is_character_a_number(char ch) {
    return ((ch >= '0') && (ch <= '9'));
}

bool evl_token::is_character_a_alpha_num_space_dollar(char ch) {
    return ((ch >= 'a') && (ch <= 'z'))
        || ((ch >= 'A') && (ch <= 'Z'))
        || ((ch >= '0') && (ch <= '9'))
        || (ch == '_') || (ch == '$');
}
 
bool evl_token::extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens) { // use reference to modify it
    std::ifstream input_file(file_name);
    if (!input_file) {
        std::cerr << "I can’t read " << file_name << "." << std::endl;
        return false;
    }
    tokens.clear(); // be defensive, make it empty
    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no) {
        if (!extract_tokens_from_line(line, line_no, tokens)) {
            return false;
        }   
    }
    return true;
}

bool evl_token::extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens) { // use reference to modify it
    for (size_t i = 0; i < line.size();) {
        //... // comments and spaces
        if (is_character_a_comment(line[i])) {
            ++i;
            if ((i == line.size()) || (line[i] != '/'))
            {
                std::cerr << "LINE " << line_no
                    << ": a single / is not allowed" << std::endl;
                return -1;
            }
            break; // skip the rest of the line by exiting the loop
        }
        // spaces
        else if (is_character_a_space(line[i])) {
            ++i; // skip this space character
            continue; // skip the rest of the iteration
        }
        // SINGLE
        else if (is_character_a_single(line[i])) {
            evl_token token_single;
            token_single.set(evl_token::SINGLE, std::string(1, line[i]), line_no);
            tokens.push_back(token_single);
            ++i;
            continue;
        }
        else if (isalpha(line[i]) || (line[i] == '_')) {
        //... // a NAME token
            size_t name_begin = i;
            for (++i; i < line.size(); ++i)
            {
                if (!is_character_a_alpha_num_space_dollar(line[i])) {
                    break; // [name_begin, i) is the range for the token
                }
            }
            evl_token token_name;
            token_name.set(evl_token::NAME, line.substr(name_begin, i-name_begin), line_no);
            tokens.push_back(token_name);
            continue;
        }
        //... // NUMBER token and error handling
        // NUMBER
        else if (is_character_a_number(line[i])) {
            size_t num = i;
            for (++i; i < line.size(); ++i)
            {
            if (!is_character_a_number(line[i]))
                break;
            }   
            evl_token token_number;
            token_number.set(evl_token::NUMBER, line.substr(num, i-num), line_no);
            tokens.push_back(token_number);
        }
        else
        {
            std::cerr << "LINE " << line_no
                << ": invalid character" << std::endl;
            return -1;
        }
    }
    return true; // nothing left
}

void evl_token::display_tokens(std::ostream &out, const std::vector<evl_token> &tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].get_token_type() == evl_token::SINGLE) {
            out << "SINGLE " << tokens[i].get_string() << std::endl;
        }
        else if (tokens[i].get_token_type() == evl_token::NAME) {
            out << "NAME " << tokens[i].get_string() << std::endl;
        }
        else { // must be NUMBER
            out << "NUMBER " << tokens[i].get_string() << std::endl;
        }
    }
}   

bool evl_token::store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens) {
    std::ofstream output_file(file_name.c_str());
    //... // verify output_file is ready
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << ".tokens ." << std::endl;
        return -1;
    }
    // almost the same loop as display_tokens
    display_tokens(output_file, tokens);
    return true;
}

bool evl_token::token_is_semicolon(const evl_token &token) {
    return token.get_string() == ";";
}

bool evl_token::has_semicolon(const std::vector<evl_token> &tokens) {
    auto next_sc = std::find_if(tokens.begin(), tokens.end(), 
        [](const evl_token &token) {
            return token.get_string() == ";";
    });
    
    return next_sc != tokens.end();
}
