#pragma once


#include "IDK_builtin_modules.h"


class idk::builtin_modules::Builtin_RenderPipeline: public idk::Module
{
private:
    GLuint      _default_shader;
    idk::Allocator<idk::Model>  _model_allocator;

public:
                Builtin_RenderPipeline(int idx): Module(idx)   {  };
    void        init(idk::Engine &);
    void        stage_A(idk::Engine &);
    void        stage_B(idk::Engine &)  {  };

    void        make_gibbable( uint obj_id, uint gibmodel_id )  {  };

};

