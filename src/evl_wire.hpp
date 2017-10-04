// evl_wire.hpp

class evl_wire {

    std::string name;

    int width;

public:

// Constructors

    evl_wire();

    evl_wire(std::string n, int w);

// Setters

    bool set(std::string n, int w);

    bool set_name(std::string n);

    bool set_width(int w);

// Getters

    std::string get_name() const;

    int get_width() const;

// Other methods

    static bool process_wire_statement(evl_wires &wires, evl_statement &s);

    static bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
 
    static void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);  

}; // class evl_wire
