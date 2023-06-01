#pragma once

#include "../IDKcore/IDKcore.h"



class IDK::ModelSystem
{
private:

    IDK::Allocator<IDK::Texture> _texture_allocator;
    IDK::Allocator<IDK::Material> _material_allocator;
    IDK::Allocator<IDK::Model> _model_allocator;


public:

    ModelSystem() {  };


};

