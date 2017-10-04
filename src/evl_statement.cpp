// evl_statement.cpp
#include "evl_statement.hpp"

evl_statement::evl_statement();

bool evl_statement::group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens) {
    for (; !tokens.empty();) { // generate one statement per iteration
        evl_token token = tokens.front();
        if (token.type != evl_token::NAME) {
            std::cerr << "Need a NAME token but found '" << token.str
                << "' on line " << token.line_no << std::endl;
            return false;
        }
        if (token.str == "module") { // MODULE statement
            //...
            evl_statement module;
            module.type = evl_statement::MODULE;
            // Thinking of a function to replace the loop?
            if (!move_tokens_to_statement(module.tokens, tokens))
                return false;
            /* for (; !tokens.empty();) {
                module.tokens.push_back(tokens.front());
                tokens.pop_front(); // consume one token per iteration
                if (module.tokens.back().str == ";")
                    break; // exit if the ending ";" is found
            }
            if (module.tokens.back().str != ";") {
                std::cerr << "Look for ’;’ but reach the end of file" << std::endl;
                return false;
            }*/

            statements.push_back(module);
            continue;
        }   
        else if (token.str == "endmodule") { // ENDMODULE statement
            //...
            evl_statement endmodule;
            endmodule.type = evl_statement::ENDMODULE;
            endmodule.tokens.push_back(token);
            tokens.pop_front();
            statements.push_back(endmodule);
        }
        else if (token.str == "wire") { // WIRE statement
            //...
            evl_statement wire;
            wire.type = evl_statement::WIRE;
            if (!move_tokens_to_statement(wire.tokens, tokens))
                return false;
            statements.push_back(wire);
        }
        else { // COMPONENT statement
            //...
            evl_statement component;
            component.type = evl_statement::COMPONENT;
            if (!move_tokens_to_statement(component.tokens, tokens)) 
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
