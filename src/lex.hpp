// lex.hpp

#include <list>
#include <vector>
#include <map>

class lex {
    
public:

    void remove_all_zeros(std::list<int> &integers);

    void show_vector(const std::vector<int> &vec);

};

class evl_token;

class evl_statement;

class evl_wire;

typedef std::list<evl_token> evl_tokens;

typedef std::list<evl_statement> evl_statements;

typedef std::vector<evl_wire> evl_wires;

typedef std::map<std::string, int> evl_wires_table;
