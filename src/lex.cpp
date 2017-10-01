#include <iostream>
#include <fstream>
#include <string>
#include <list>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file)
    {
        std::cerr << "I can't read " << argv[1] << "." << std::endl;
        return -1;
    }

    std::string output_file_name = std::string(argv[1])+".tokens";
    std::ofstream output_file(output_file_name);
    if (!output_file)
    {
        std::cerr << "I can't write " << argv[1] << ".tokens ." << std::endl;
        return -1;
    }

    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no)
    {
        for (size_t i = 0; i < line.size();)
        {
            // comments
            if (line[i] == '/')
            {
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
            if ((line[i] == ' ') || (line[i] == '\t')
                || (line[i] == '\r') || (line[i] == '\n'))
            {
                ++i; // skip this space character
                continue; // skip the rest of the iteration
            }

            // SINGLE
            if ((line[i] == '(') || (line[i] == ')')
                || (line[i] == '[') || (line[i] == ']')
                || (line[i] == ':') || (line[i] == ';')
                || (line[i] == ','))
            {
                output_file << "SINGLE " << line[i] << std::endl;
                ++i; // we consumed this character
                continue; // skip the rest of the iteration
            }

            // NAME
            if (((line[i] >= 'a') && (line[i] <= 'z'))       // a to z
                || ((line[i] >= 'A') && (line[i] <= 'Z'))    // A to Z
                || (line[i] == '_'))
            {
                size_t name_begin = i;
                for (++i; i < line.size(); ++i)
                {
                    if (!(((line[i] >= 'a') && (line[i] <= 'z'))
                        || ((line[i] >= 'A') && (line[i] <= 'Z'))
                        || ((line[i] >= '0') && (line[i] <= '9'))
                        || (line[i] == '_') || (line[i] == '$')))
                    {
                        break; // [name_begin, i) is the range for the token
                    }
                }
                output_file << "NAME "
                    << line.substr(name_begin, i-name_begin) << std::endl;
                continue;
            }

            // NUMBER
            if ((line[i] >= '0') && (line[i] <= '9'))
            {
                size_t num = i;
                for (++i; i < line.size(); ++i)
                {
                    if (!((line[i] >= '0') && (line[i] <= '9')))
                        break;
                }
                output_file << "NUMBER " << line.substr(num, i-num) << std::endl;
            }

            else
            {
                std::cerr << "LINE " << line_no
                    << ": invalid character" << std::endl;
                return -1;
            }
        }
    }

    struct evl_token {
        enum token_type {NAME, NUMBER, SINGLE};
        token_type type;
        std::string str;
        int line_no;
    }; // struct evl_token

    typedef std::list<evl_token> evl_tokens;

    struct evl_statement {
        enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};
        statement_type type;
        evl_tokens tokens;
    }; // struct evl_statement

    typedef std::list<evl_statement> evl_statements;

    //... // verify that argv[1] exists
    std::string evl_file = argv[1];
    evl_tokens tokens;
    if (!extract_tokens_from_file(evl_file, tokens)) {
        return -1;
    }
    display_tokens(tokens);
    if (!store_tokens_to_file(evl_file+".tokens", tokens)) {
        return -1;
    }
    evl_statements statements;
    if (!group_tokens_into_statements(statements, tokens)) {
        return -1;
    }
    display_statements(statements);

    bool group_tokens_into_statements(
        evl_statements &statements,
        evl_tokens &tokens) {
        //...
        return true;
    }

    bool group_tokens_into_statements(
        evl_statements &statements, evl_tokens &tokens) {
        for (; !tokens.empty();) { // generate one statement per iteration
            evl_token token = tokens.front();
            if (token.type != evl_token::NAME) {
            std::cerr << "Need a NAME token but found ’" << token.str
                << "’ on line " << token.line_no << std::endl;
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
            }
            else { // COMPONENT statement
                //...
            }
        }
    }

    bool move_tokens_to_statement(
        evl_tokens &statement_tokens,
        evl_tokens &tokens) {
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

    struct evl_wire {
        std::string name;
        int width;
    }; // struct evl_wire

    typedef std::vector<evl_wire> evl_wires;

    bool process_wire_statement(evl_wires &wires, evl_statement &s) {
        //...
        enum state_type {INIT, WIRE, DONE, WIRES, WIRE_NAME};
        state_type state = INIT;
        for (; !s.tokens.empty() && (state != DONE); s.tokens.pop_front()) {
            evl_token t = s.tokens.front();
            //... // use branches here to compute state transitions
            if (state == INIT) {
                //...
                if (t.str == "wire") {
                    state = WIRE;
                }
                else {
                    std::cerr << "Need ’wire’ but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
            else if (state == WIRE) {
                //...
                if (t.type == evl_token::NAME) {
                    evl_wire wire;
                    wire.name = t.str; wire.width = 1;
                    wires.push_back(wire);
                    state = WIRE_NAME;
                }
                else {
                    std::cerr << "Need NAME but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
            else if (state == WIRES) {
                //...
                //... // same as the branch for WIRE
            }
            else if (state == WIRE_NAME) {
                //...
                if (t.str == ",") {
                    state = WIRES;
                }
                else if (t.str == ";") {
                    state = DONE;
                }
                else {
                    std::cerr << "Need ’,’ or ’;’ but found ’" << t.str
                        << "’ on line " << t.line_no << std::endl;
                    return false;
                }
            }
        }
        if (!s.tokens.empty() || (state != DONE)) {
            std::cerr << "something wrong with the statement" << std::endl;
            return false;
        }
        return true;
    }
/* 
    int bus_width = 1;
        //...
    for (...) {
        //...
        else if (state == BUS) {
            if (t.type == evl_token::NUMBER) {
                bus_width = atoi(t.str.c_str())+1;
                state = BUS_MSB;
            }
            //...
        }
        //...
    }
    //...

*/
    return 0;
}
