#pragma once

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/IDKGraphics.hpp>


namespace idkg::ui
{
    void transform_component( idk::Engine &engine, idk::Camera &camera, idk::Transform_CS &CS,
                              int obj_id, float tsnap, float rsnap );
};


