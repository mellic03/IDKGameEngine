#ifdef __unix__
  #define ENTRY main
#elif defined(_WIN32) || defined(WIN32)
  #define ENTRY WinMain
#endif

#include <iostream>
#include "IDKengine.h"



int ENTRY(int argc, const char **argv)
{
    idk::Engine engine;

    // idk::graph graph(11825);
    // std::ifstream instream("finalInput.txt");
    // std::string line;

    // while (getline(instream, line))
    // {
    //     int v0, v1;
    //     float weight;
    //     std::istringstream iss(line);
    //     iss >> v0 >> v1 >> weight;
    //     graph.add_edge(v0, v1, weight);
    //     graph.add_edge(v1, v0, weight);
    // }

    // clock_t time = clock();
    // auto v = graph.dijkstra(0, 5);
    // time = clock() - time;
    // std::cout << "time: " << ((double)time) / CLOCKS_PER_SEC << "\n";

    // std::cout << "path: ";
    // for (int n: v)
    //     std::cout << n << " ";
    // std::cout << "\n";

    GLuint shader_id = engine.rengine().compileShaderProgram("assets/shaders/", "screenquad.vs", "mouse.fs");
    uint model_id  = engine.rengine().loadOBJ("assets/models/cube/", "cube.obj", "cube.mtl");
    uint transform_id = engine.rengine().createTransform();


    while (engine.running())
    {
        engine.beginFrame();

        engine.rengine().useShaderProgram(shader_id);
        engine.rengine().drawModel(model_id, transform_id);

        engine.endFrame();
    }

    return 0;
}

