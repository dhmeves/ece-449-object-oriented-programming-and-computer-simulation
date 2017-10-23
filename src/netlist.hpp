// netlist.hpp
#ifndef GUARD_NETLIST_H
#define GUARD_NETLIST_H

class netlist {

    std::list<gate *> gates_;

    std::list<net *> nets_;

    std::map<std::string, net *> nets_table_;

}; // class netlist

#endif
