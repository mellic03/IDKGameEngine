#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"



int ENTRY(int argc, const char **argv)
{
    IDK::Engine idk;

    std::string shaderpath = "assets/shaders/";

    IDK::Shader shader1("assets/shaders/", "screenquad.vs", "screenquad.fs");
    IDK::Model model1("assets/", "cube.obj", "cube.mtl");
    IDK::transform transform;


    IDK::BST<int, int> bst;

    for (int i=0; i<10; i++)
    {
        bst.insert(rand()%100, i);
    }

    bst.print();


    while (idk.running())
    {
        idk.beginFrame();



        idk.endFrame();
    }

    return 0;
}

