#include <list>
#include <vector>

typedef std::list<evl_token> evl_tokens;

typedef std::list<evl_statement> evl_statements;

typedef std::vector<evl_wire> evl_wires;

#include "evl_token.hpp"
#include "evl_statement.hpp"
#include "evl_wire.hpp"

void remove_all_zeros(std::list<int> &integers);

void show_vector(const std::vector<int> &vec);
