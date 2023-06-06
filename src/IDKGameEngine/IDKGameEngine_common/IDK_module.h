#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::Module
{
protected:

public:
    virtual void beginFrame(idk::Engine &engine) = 0;
    virtual void endFrame(idk::Engine &engine) = 0;
};



class example_module: public idk::Module
{
private:

public:
    void beginFrame(idk::Engine &engine)
    {
        std::cout << "begin frame\n";
    };

    void endFrame(idk::Engine &engine)
    {
        std::cout << "end frame\n";
    };

};

