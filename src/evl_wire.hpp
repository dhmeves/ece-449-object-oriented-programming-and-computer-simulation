class evl_wire {

    std::string name;

    int width;

public:

    evl_wire(std::string n, int w);

    bool set(std::string n, int w);

    std::string get_name();

    int get_width();

    bool process_wire_statement(evl_wires &wires, evl_statement &s);

    bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
 
    void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);  

}; // class evl_wire
