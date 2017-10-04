#include <string>

class evl_token {

    enum token_type {NAME, NUMBER, SINGLE};
    
    token_type type;
    
    std::string str;
    
    int line_no;

public:

    evl_token(token_type t, std::string s, int l);

    bool set(token_type t, std::string s, int l);
   
    token_type get_token_type();

    std::string get_string();

    int get_line_no();
 
    bool is_character_a_comment(char ch);
       
    bool is_character_a_space(char ch);

    bool is_character_a_single(char ch); 

    bool is_character_a_name(char ch);

    bool is_character_a_number(char ch);

    bool is_character_a_alpha_num_space_dollar(char ch);

    bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens);

    bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens);

    void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens);

    bool store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens);

    bool token_is_semicolon(const evl_token &token);

    bool has_semicolon(const evl_tokens &tokens);

}; // class evl_token
