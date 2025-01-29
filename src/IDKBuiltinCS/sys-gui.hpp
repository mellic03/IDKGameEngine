#pragma once

#include <libidk/idk_glm.hpp>
#include <string>

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKECS/IDKECS.hpp>
#include "idk_components.hpp"


namespace idk
{
    class GUIButtonCmp;
    class GUIWorldSpaceCmp;
    class GUISys;
}


class idk::GUISys: public idk::ECS::System 
{
public:
    virtual void init   ( idk::EngineAPI& ) final;
    virtual void update ( idk::EngineAPI& ) final;

    int createButton( const std::string &label, const std::function<void()> &callback );
    idkui::Button *getButton( int obj );

    void renderTexture( idk::EngineAPI&, idkui::LayoutManager* );
};


struct idk::GUIButtonCmp
{
    int obj_id = -1;
    idkui::Button *element;
    idkui::Style style1;
    idkui::Style style2;

    std::string label = "";
    glm::vec2 position = glm::vec2(512, 512);

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
};
