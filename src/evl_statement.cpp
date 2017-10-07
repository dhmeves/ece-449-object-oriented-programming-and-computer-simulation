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

#include "lex.hpp"
#include "evl_token.hpp"
#include "evl_statement.hpp"
#include "evl_wire.hpp"

evl_statement::evl_statement() {}

evl_statement::evl_statement(statement_type t, evl_tokens tok) : type(t), tokens(tok) {}

bool evl_statement::set(statement_type t, evl_tokens tok) {
    //,,,// return false if statement is not valid
    type = t;
    tokens = tok;
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

evl_statement::statement_type evl_statement::get_statement_type() const{
    return type;
}

evl_tokens evl_statement::get_evl_tokens() {
    return tokens;
}

bool evl_statement::group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens) {
    for (; !tokens.empty();) { // generate one statement per iteration
        evl_token token = tokens.front();
        if (token.get_token_type() != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << token.get_string()
                << "' on line " << token.get_line_no() << std::endl;
            return false;
        }
        if (token.get_string() == "module") { // MODULE statement
            //...
            evl_tokens token_list; 
            evl_statement *module = new evl_statement(evl_statement::MODULE, token_list);
            // Thinking of a function to replace the loop?
            if (!move_tokens_to_statement(module.get_evl_tokens(), tokens))
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
            evl_tokens token_list;
            evl_statement *endmodule = new evl_statement(evl_statement::ENDMODULE, token_list);
            endmodule.get_evl_tokens().push_back(token);
            tokens.pop_front();
            statements.push_back(endmodule);
        }
        else if (token.get_string() == "wire") { // WIRE statement
            //...
            evl_statement *wire = new evl_statement(evl_statement::WIRE, token_list);
            if (!move_tokens_to_statement(wire.get_evl_tokens(), tokens))
                return false;
            statements.push_back(wire);
        }
        else { // COMPONENT statement
            //...
            evl_statement *component = new evl_statement(evl_statement::COMPONENT, token_list);
            if (!move_tokens_to_statement(component.get_evl_tokens(), tokens)) 
                return false;
            statements.push_back(component);
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

void evl_statement::display_statements(std::ostream &out, const std::vector<evl_statement> &statements) {
    for (size_t i = 0; i < statements.size(); ++i) {
        if (statements[i].get_statement_type() == evl_statement::MODULE) {
            out << "module " << statements[i].get_evl_tokens().begin().get_string() << std::endl;
        }
        else if (statements[i].get_statement_type() == evl_statement::WIRE) {
            out << "wire " << statements[i].get_evl_tokens().begin().get_string() << std::endl;
        }
        else if (statements[i].get_statement_type() == evl_statement::COMPONENT) {
            out << "component " << statements[i].get_evl_tokens().begin().get_string() << std::endl;
        }
    }
}   

bool evl_statement::store_statements_to_file(std::string file_name, const std::vector<evl_statement> &statements) {
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
