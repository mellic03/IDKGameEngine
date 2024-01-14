#include "idk_icon_cs.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"


void
idk::Icon_CS::init( idk::EngineAPI &api )
{

}


void
idk::Icon_CS::setIcon( int obj_id, const std::string &icon )
{
    int key = m_keys[obj_id];
    m_icons.get(key) = icon; 
}


std::string &
idk::Icon_CS::getIcon( int obj_id )
{
    int key = m_keys[obj_id];

    if (key == -1)
    {
        static std::string temp = "Object does not exist";
        return temp;
    }

    return m_icons.get(key);
}


void
idk::Icon_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_keys[obj_id] = m_icons.create(m_default_icon);
}


void
idk::Icon_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    engine.giveComponent(obj_id, this->ID());
}


void
idk::Icon_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    int &key = m_keys[obj_id];
    m_icons.destroy(key);
    key = -1;
}


void
idk::Icon_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    // engine.giveComponent(dest_obj_id, this->m_id);
    setIcon(dest_obj_id, getIcon(src_obj_id));
}


void
idk::Icon_CS::onObjectSelection( int obj_id )
{
    // static const std::vector<std::string> icons = {
    //     IDK_ICON_FILE,
    //     IDK_ICON_FOLDER,
    //     IDK_ICON_FOLDER_OPENED,
    //     IDK_ICON_PERSON,
    //     IDK_ICON_EYE,
    //     IDK_ICON_EYE_SLASH,
    //     IDK_ICON_CAMERA,
    //     IDK_ICON_TRASH_CAN,
    //     IDK_ICON_PLUS,
    //     IDK_ICON_BOX,
    //     IDK_ICON_BOX_OPEN,
    //     IDK_ICON_FOLDER_TREE,
    //     IDK_ICON_GLOBE,
    //     IDK_ICON_MOUNTAIN,
    //     IDK_ICON_MOUNTAIN_CITY,
    //     IDK_ICON_MOUNTAIN_SUN,
    //     IDK_ICON_TREE,
    //     IDK_ICON_TREE_CITY,
    //     IDK_ICON_BUILDING,
    //     IDK_ICON_BUILDING_COLUMNS
    // };

    std::string &icon = getIcon(obj_id);


    size_t NUM_COLS = size_t(ImGui::GetWindowWidth() / ImGui::GetFontSize()) / 2;
           NUM_COLS = (NUM_COLS <= 0) ? 1 : NUM_COLS;

    if (ImGui::BeginTable("Icons", NUM_COLS))
    {
        ImGui::TableNextRow();
        
        for (size_t i=0; i<IDK_ICONS.size(); i++)
        {
            if (i % NUM_COLS == 0)
            {
                ImGui::TableNextRow;
            }

            ImGui::TableNextColumn();

            bool selected = icon == IDK_ICONS[i];

            if (ImGui::Selectable(IDK_ICONS[i], &selected))
            {
                icon = IDK_ICONS[i];
            }

        }
    
        ImGui::EndTable();
    }



    // for (int i=0; i<icons.size(); i++)
    // {
    //     bool selected = icon == icons[i];

    //     if (ImGui::Selectable(icons[i].c_str(), &selected))
    //     {
    //         icon = icons[i];
    //     }
    // }
}
