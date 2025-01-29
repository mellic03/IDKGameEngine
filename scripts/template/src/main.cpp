#include <iostream>

#include <IDKGraphics/UI/idk_ui.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKIO/IDKIO.hpp>

#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/sys-script.hpp>
#include <IDKBuiltinCS/sys-gui.hpp>
#include <IDKPhysics/IDKPhysics.hpp>

#include "game.hpp"

// #include <IDKEditorUI/EditorUI.hpp>
#include <IDKBuiltinUI/EditorUI.hpp>


// void
// TemplateGameName::registerModules( idk::EngineAPI &api )
// {
//     using namespace idk;
//     int id = api.getEngine().registerModule("IDKGE/modules/libIDKBuiltinUI");
//     auto &mod = api.getEngine().getModule<EditorUI_MD>(id);

// }


void
TemplateGameName::setup( const std::vector<std::string> &args, idk::EngineAPI &api )
{

}


void
TemplateGameName::mainloop( idk::EngineAPI &api )
{

}


void
TemplateGameName::shutdown()
{
    LOG_INFO() << "TemplateGameName::shutdown";
}


void
TemplateGameName::build()
{

}