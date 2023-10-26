#pragma once

#include <iostream>
#include <map>


namespace idk { template <typename key_t, typename value_t> class map; };


template <typename key_t, typename value_t>
class idk::map
{
private:
    struct Handle
    {
        bool exists = false;
        value_t value;
    };

    std::map<key_t, Handle> m_hashtable;

public:

    void put( key_t key, value_t value )
    {
        m_hashtable[key].value = value;
        m_hashtable[key].exists = true;
    };

    T &get( key_t key )
    {
        Handle h = m_hashtable[key];
        
        #ifdef IDK_DEBUG
            if (h.exists == false)
            {
                std::cout << "[idk::map::get] Key \"" << key << "\"has no associated value!\n";
                exit(1);
            }
        #endif

        return h.value;
    };

};
