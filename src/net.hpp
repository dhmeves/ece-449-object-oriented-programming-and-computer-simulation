// net.hpp
#ifndef GUARD_NET_H
#define GUARD_NET_H

class net {
    
    std::string name_; // e.g. "a",class net {
    
    std::string name_; // e.g. "a", "s[0]"
    
    char signal_; // e.g. ’0’ or ’1’
    
    std::list<pin *> connections_; // relationship "connect"

}; // class net

#endif
