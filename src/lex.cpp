// lex.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <map>

#include "lex.hpp"
#include "evl_token.hpp"
#include "evl_statement.hpp"
#include "evl_wire.hpp"

void lex::remove_all_zeros(std::list<int> &integers) {
    auto to_be_erased = std::remove_if(
        integers.begin(), integers.end(),
        [](int k) {return k == 0;});
    integers.erase(to_be_erased, integers.end());
}

void lex::show_vector(const std::vector<int> &vec) {
    for (size_t i = 1; i <= vec.size(); ++i) {
        assert(i < vec.size());
        std::cout << vec[i] << std::endl;
    }
}

/* 
    int bus_width = 1;
        //...
    for (...) {
        //...
        else if (state == BUS) {
            if (t.type == evl_token::NUMBER) {
                bus_width = atoi(t.str.c_str())+1;
                state = BUS_MSB;
            }
            //...
        }
        //...
    }
    //...

*/ 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }
    //... // verify that argv[1] exists
    std::string evl_file = argv[1];
    std::vector<evl_token> tokens;
    if (!evl_token::extract_tokens_from_file(evl_file, tokens)) {
        return -1;
    }
    evl_token::display_tokens(std::cout, tokens); // why we need it?
    if (!evl_token::store_tokens_to_file(evl_file+".tokens", tokens)) {
        return -1;
    }
    // Copy token vector to token list and print out 
    evl_tokens token_list;
    std::copy(tokens.begin(), tokens.end(), std::back_inserter(token_list));
    for (auto v : token_list)
        std::cout << v.get_token_type() << v.get_string() << v.get_line_no() << "\n";
   
    evl_statements statements;
    if (!evl_statement::group_tokens_into_statements(statements, token_list)) {
        return -1;
    }
    std::vector<evl_statement> statement_vec{std::begin(statements), std::end(statements)};
    evl_statement::display_statements(std::cout, statement_vec);
    if (!evl_statement::store_statements_to_file(evl_file+".syntax", statement_vec)) {
        return -1;
    }
    return 0;
}
