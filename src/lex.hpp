#include <list>
#include <vector>

typedef std::list<evl_token> evl_tokens;

typedef std::list<evl_statement> evl_statements;

bool token_is_semicolon(const evl_token &token);

bool has_semicolon(const evl_tokens &tokens);

void remove_all_zeros(std::list<int> &integers);

void show_vector(const std::vector<int> &vec);

typedef std::vector<evl_wire> evl_wires;
