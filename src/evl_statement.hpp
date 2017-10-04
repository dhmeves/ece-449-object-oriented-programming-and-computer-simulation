// evl_statement.hpp

class evl_statement {

public:

    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};
    
private:

    statement_type type;
    
    evl_tokens tokens;

public:
        
    evl_statement();

    evl_statement(statement_type t, evl_tokens tok);

    bool set(statement_type t, evl_tokens tok);

    bool set_statement_type(statement_type t);

    bool set_evl_tokens(evl_tokens tok);

    statement_type get_statement_type() const;

    evl_tokens & get_evl_tokens() const;

    bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

    bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

}; // class evl_statement
