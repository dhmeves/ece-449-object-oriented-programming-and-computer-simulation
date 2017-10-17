// evl_component.hpp

#include "evl_token.hpp"

class evl_component {

    evl_token::token_type type;    

    std::string name;
    
public:    

// Constructors

    evl_component();

    evl_component(evl_token::token_type t, std::string n);

// Setters

    bool set(evl_token::token_type t, std::string n);

    bool set_type(evl_token::token_type t);

    bool set_name(std::string n);

// Getters

    evl_token::token_type get_type() const;

    std::string get_name() const;

// Other Methods

    static bool process_component_statement(evl_components &components, evl_statement &s);

};
