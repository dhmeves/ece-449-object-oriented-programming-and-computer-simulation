// evl_statement.cpp
#include "lex.hpp"
#include "evl_statement.hpp"
#include "evl_token.hpp"

evl_statement::evl_statement() {}

evl_statement::evl_statement(statement_type t, evl_tokens tok) : type(t), tokens(tok) {}

bool set(statement_type t, evl_tokens tok) {
    //,,,// return false if statement is not valid
    type = t;
    tokens = tok;
    return true;
}

bool set_statement_type(statement_type t) {
    //...// return false if statment type is invalid
    type = t;
    return true;
}

bool set_evl_tokens(evl_tokens tok) {
    //...// return false if tokens are invalid
    tokens = tok;
    return true;
}

statement_type get_statement_type() const{
    return type;
}

evl_tokens & get_evl_tokens() const{
    return tokens;
}

bool evl_statement::group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens) {
    for (; !tokens.empty();) { // generate one statement per iteration
        evl_token token = tokens.front();
        if (token.get_token_type() != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << token.get_str()
                << "' on line " << token.get_line_no() << std::endl;
            return false;
        }
        if (token.get_string() == "module") { // MODULE statement
            //...
            evl_statement module;
            module.set_statement_type(evl_statement::MODULE);
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
            evl_statement endmodule;
            endmodule.set_statement_type(evl_statement::ENDMODULE);
            endmodule.get_statement_type() = evl_statement::ENDMODULE;
            endmodule.get_evl_tokens().push_back(token);
            tokens.pop_front();
            statements.push_back(endmodule);
        }
        else if (token.get_string() == "wire") { // WIRE statement
            //...
            evl_statement wire;
            wire.set_statement_type(evl_statement::WIRE);
            if (!move_tokens_to_statement(wire.get_evl_tokens(), tokens))
                return false;
            statements.push_back(wire);
        }
        else { // COMPONENT statement
            //...
            evl_statement component;
            component.set_statement_type(evl_statement::COMPONENT);
            if (!move_tokens_to_statement(component.get_evl_tokens(), tokens)) 
                return false;
            statements.push_back(component);
        }
    }
}

bool evl_statement::move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens) {
    // What if someone passes the two parameters in the wrong order?
    assert(statement_tokens.empty());
    // search for ";"
    evl_tokens::iterator next_sc = std::find_if(
        tokens.begin(), tokens.end(), token_is_semicolon);
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
