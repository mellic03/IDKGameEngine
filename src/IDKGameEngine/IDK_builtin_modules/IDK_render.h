#pragma once


#include "../IDK_engine.h"


class Builtin_RenderPipeline: public idk::Module
{
private:
    GLuint      _default_shader;
    idk::Allocator<idk::Model>  _model_allocator;

public:
                Builtin_RenderPipeline( std::string name, int idx ): Module(name, idx)   {  };
    void        init(idk::Engine &);
    void        stage_A(idk::Engine &);
    void        stage_B(idk::Engine &)  {  };

    void        make_gibbable( uint obj_id, uint gibmodel_id )  {  };

};

