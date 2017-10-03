#include <string>

class evl_token {

    enum token_type {NAME, NUMBER, SINGLE};
    
    token_type type;
    
    std::string str;
    
    int line_no;

public:

    evl_token();

    bool extract_tokens_from_file(std::string file_name, std::vector<evl_token> &tokens);

    bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens);

    void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens);

    bool store_tokens_to_file(std::string file_name, const std::vector<evl_token> &tokens);

} // class evl_token
