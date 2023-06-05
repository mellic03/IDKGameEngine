#pragma once

#include "../IDKcore/IDKcore.h"


class idk::Navmap
{
private:
    idk::vector<idk::vector<int>>   _paths;

public:
                                    Navmap() {  };
    idk::vector<int>                path(int a, int b);

};

