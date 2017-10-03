#include "evl_token.hpp"

class evl_token {
    
    enum token_type {NAME, NUMBER, SINGLE};
    
    token_type type;
    
    std::string str;
    
    int line_no;

public:
    
    evl_token();

    bool is_character_a_comment(char ch) {
        return (ch == '/');
    }    

    bool is_character_a_space(char ch) {
        return (ch == ' ') || (ch == '\t')
            || (ch == '\r') || (ch == '\n');
    }

    bool is_character_a_single(char ch) {
        return (ch == '(') || (ch == ')')
            || (ch == '[') || (ch == ']')
            || (ch == ':') || (ch == ';')
            || (ch == ','); 
    }

    bool is_character_a_name(char ch) {
        return ((ch >= 'a') && (ch <= 'z'))    // a to z
            || ((ch >= 'A') && (ch <= 'Z'))    // A to Z
            || (ch == '_'); 
    }

    bool is_character_a_number(char ch) {
        return ((ch >= '0') && (ch <= '9'));
    }

    bool is_character_a_alpha_num_space_dollar(char ch) {
        return ((ch >= 'a') && (ch <= 'z'))
            || ((ch >= 'A') && (ch <= 'Z'))
            || ((ch >= '0') && (ch <= '9'))
            || (ch == '_') || (ch == '$');
    }
     
    bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens) { // use reference to modify it
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

    bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens) { // use reference to modify it
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
                evl_token token;
                token.line_no = line_no;
                token.type = evl_token::SINGLE;
                token.str = std::string(1, line[i]);
                tokens.push_back(token);
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
                evl_token token;
                token.line_no = line_no;
                token.type = evl_token::NAME;
                token.str = line.substr(name_begin, i-name_begin);
                tokens.push_back(token);
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
                evl_token token;
                token.line_no = line_no;
                token.type = evl_token::NUMBER;
                token.str = line.substr(num, i-num);
                tokens.push_back(token);
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

    void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens) {
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].type == evl_token::SINGLE) {
                out << "SINGLE " << tokens[i].str << std::endl;
            }
            else if (tokens[i].type == evl_token::NAME) {
                out << "NAME " << tokens[i].str << std::endl;
            }
            else { // must be NUMBER
                out << "NUMBER " << tokens[i].str << std::endl;
            }
        }
    }   
    
    bool store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens) {
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

    bool token_is_semicolon(const evl_token &token) {
        return token.str == ";";
    }

    bool has_semicolon(const std::vector<evl_token> &tokens) {
        auto next_sc = std::find_if(tokens.begin(), tokens.end(), 
            [](const evl_token &token) {
                return token.str == ";";
        });
        
        return next_sc != tokens.end();
    }


} // class evl_token




