#pragma once

#include <IDKBuiltinCS/IDKBuiltinCS.hpp>
#include <IDKGraphics/IDKGraphics.hpp>


namespace idk::ui
{
    void transform_component( idk::EngineAPI &api, idk::Camera &camera, idk::Transform_CS &CS,
                              glm::mat4 &model, glm::mat4 transform, glm::mat4 parent, float tsnap, float rsnap );


    void transform_component2( idk::EngineAPI &api, idk::Camera &camera, idk::Transform_CS &CS,
                              glm::mat4 &model, glm::mat4 transform, glm::mat4 parent );



};


