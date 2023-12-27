#pragma once

#include <libidk/IDKcore/idk_identifiable.hpp>
#include <libidk/IDKcore/idk_initializable.hpp>
#include <libidk/IDKcore/idk_resettable.hpp>

#include <string>
#include <vector>


namespace idk
{
    class GameObject;
};




class idk::GameObject: public idk::Identifiable,
                       public idk::Resettable<int, size_t>
{
private:

public:

};

