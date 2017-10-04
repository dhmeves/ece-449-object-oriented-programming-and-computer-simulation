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

void remove_all_zeros(std::list<int> &integers) {
    auto to_be_erased = std::remove_if(
        integers.begin(), integers.end(),
        [](int k) {return k == 0;});
    integers.erase(to_be_erased, integers.end());
}

void show_vector(const std::vector<int> &vec) {
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

/*int old_main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file)
    {
        std::cerr << "I can't read " << argv[1] << "." << std::endl;
        return -1;
    }

    std::string output_file_name = std::string(argv[1])+".tokens";
    std::ofstream output_file(output_file_name);
    if (!output_file)
    
        std::cerr << "I can't write " << argv[1] << ".tokens ." << std::endl;
        return -1;
    }

    std::string line;
    for (int line_no = 1; std::getline(input_file, line); ++line_no)
    {
        for (size_t i = 0; i < line.size();)
        {
            // comments
            if (line[i] == '/')
            {
                ++i;
                if ((i == line.size()) || (line[i] != '/'))
                {
                    std::cerr << "LINE " << line_no
                        << ": a single / is not allowed" << std::endl;
                    return -1;
                }
                break; // skip the rest of the line by exiting the loop
            }

            // spaces
            if ((line[i] == ' ') || (line[i] == '\t')
                || (line[i] == '\r') || (line[i] == '\n'))
            {
                ++i; // skip this space character
                continue; // skip the rest of the iteration
            }

            // SINGLE
            if ((line[i] == '(') || (line[i] == ')')
                || (line[i] == '[') || (line[i] == ']')
                || (line[i] == ':') || (line[i] == ';')
                || (line[i] == ','))
            {
                output_file << "SINGLE " << line[i] << std::endl;
                ++i; // we consumed this character
                continue; // skip the rest of the iteration
            }

            // NAME
            if (((line[i] >= 'a') && (line[i] <= 'z'))       // a to z
                || ((line[i] >= 'A') && (line[i] <= 'Z'))    // A to Z
                || (line[i] == '_'))
            {
                size_t name_begin = i;
                for (++i; i < line.size(); ++i)
                {
                    if (!(((line[i] >= 'a') && (line[i] <= 'z'))
                        || ((line[i] >= 'A') && (line[i] <= 'Z'))
                        || ((line[i] >= '0') && (line[i] <= '9'))
                        || (line[i] == '_') || (line[i] == '$')))
                    {
                        break; // [name_begin, i) is the range for the token
                    }
                }
                output_file << "NAME "
                    << line.substr(name_begin, i-name_begin) << std::endl;
                continue;
            }

            // NUMBER
            if ((line[i] >= '0') && (line[i] <= '9'))
            {
                size_t num = i;
                for (++i; i < line.size(); ++i)
                {
                    if (!((line[i] >= '0') && (line[i] <= '9')))
                        break;
                }
                output_file << "NUMBER " << line.substr(num, i-num) << std::endl;
            }

            else
            {
                std::cerr << "LINE " << line_no
                    << ": invalid character" << std::endl;
                return -1;
            }
        }
    }

    return 0;
}*/

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }
    //... // verify that argv[1] exists
    std::string evl_file = argv[1];
    evl_token token;
    std::vector<evl_token> tokens;
    if (!token.extract_tokens_from_file(evl_file, tokens)) {
        return -1;
    }
    token.display_tokens(std::cout, tokens); // why we need it?
    if (!token.store_tokens_to_file(evl_file+".tokens", tokens)) {
        return -1;
    }
    evl_tokens token_list;
    std::copy(tokens.begin(), tokens.end(), std::back_inserter(token_list));
    for (auto v : token_list)
        std::cout << v.get_token_type() << v.get_string() << v.get_line_no() << "\n";
   
    evl_statement statement;
    evl_statements statements;
    if (!statement.group_tokens_into_statements(statements, token_list)) {
        return -1;
    }
    //display_statements(statements);
    return 0;
}
