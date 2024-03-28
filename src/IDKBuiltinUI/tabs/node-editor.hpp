#pragma once

#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>
#include <libidk/idk_export.hpp>

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>


namespace idk
{
    struct EditorNode;

    enum class NodeType: uint32_t
    {
        OBJECT    = 0,
        POSITION  = 1,
        SCRIPT    = 2,
        NUMBER    = 3,
        CONDITION = 4
    };

    enum class NodeCondition: uint32_t
    {
        EQUAL        = 0,
        LESS_THAN    = 1,
        GREATER_THAN = 2,
    };

};


struct idk::EditorNode
{
    int         obj_id = -1;
    NodeType    type   = NodeType::OBJECT;
    std::string name   = "Node";


    float         condvalue = 0.0f;
    NodeCondition condition = NodeCondition::EQUAL;


    EditorNode( NodeType _type, const std::string &_name )
    :   type(_type),
        name(_name)
    {

    };

};



void idk_NodeEditor( idk::EngineAPI &api );

