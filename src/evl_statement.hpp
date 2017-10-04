class evl_statement {
   
    enum statement_type {MODULE, WIRE, COMPONENT, ENDMODULE};
    
    statement_type type;
    
    evl_tokens tokens;

public:

    evl_statement(statement_type t, evl_tokens tok);

    bool set(statement_type t, evl_tokens tok);

    statement_type get_statement_type();

    evl_tokens get_evl_tokens();

    bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);

    bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);

}; // class evl_statement
