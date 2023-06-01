#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"


class Actor: public IDK::GameObject::Base
{
private:

public:
    void testfn()
    {
        std::cout << "Actor\n";
    };
};


class Terrain: public IDK::GameObject::Base
{
private:

public:
    void testfn()
    {
        std::cout << "Terrain\n";
    };
};


int ENTRY(int argc, const char **argv)
{
    IDK::Engine idk;

    IDK::Shader shader1("assets/shaders/", "screenquad.vs", "screenquad.fs");
    IDK::Model model1("assets/", "cube.obj", "cube.mtl");
    IDK::transform transform;


    while (1)
    {
        idk.beginFrame();



        idk.endFrame();
    }

    return 0;
}

