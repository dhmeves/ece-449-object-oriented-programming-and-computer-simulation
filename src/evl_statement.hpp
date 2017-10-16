// evl_statement.hpp

class evl_statement {

public:

    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};
    
private:

    statement_type type;
    
    evl_tokens tokens;

public:

// Constructors
        
    evl_statement();

    evl_statement(statement_type t, evl_tokens tok);

// Setters

    bool set(statement_type t, evl_tokens tok);

    bool set_statement_type(statement_type t);

    bool set_evl_tokens(evl_tokens tok);

// Getters

    statement_type get_statement_type() const;

    evl_tokens get_evl_tokens() const;

    evl_tokens & get_evl_tokens_ref();

// Other methods

    static bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

    static bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

    static void display_statements(std::ostream &out, std::vector<evl_statement> &statements); 

    static bool store_statements_to_file(std::string file_name, std::vector<evl_statement> &statements); 

}; // class evl_statement
