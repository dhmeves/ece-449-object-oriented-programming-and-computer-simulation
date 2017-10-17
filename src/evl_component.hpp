// evl_component.hpp

class evl_component {

    evl_token::token_type type;    

    std::string name;
    
    evl_pins pin_vector;    
    
public:    

// Constructors

    evl_component();

    evl_component(evl_token::token_type t, std::string n, evl_pins pv);

// Setters

    bool set(evl_token::token_type t, std::string n, evl_pins pv);

    bool set_type(evl_token::token_type t);

    bool set_name(std::string n);

    bool set_pin_vector(evl_pins pv);

// Getters

    evl_token::token_type get_type() const;

    std::string get_name() const;

    evl_pins get_pin_vector() const;

    evl_pins & get_pin_vector_ref();

// Other Methods

    static bool process_component_statement(evl_components &components, evl_statement &s);

};
