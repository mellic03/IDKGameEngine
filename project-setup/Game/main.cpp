#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"
#include "Modules/idk_modules.hpp"




int ENTRY(int argc, const char **argv)
{
    idk::Engine engine("IDK Game", 1920, 1080, 1);
    idk::RenderEngine &ren = engine.rengine();

    int dirlight_id = ren.createDirlight();

    while (engine.running())
    {
        engine.beginFrame();



        engine.endFrame();
    }


    return 0;
}
