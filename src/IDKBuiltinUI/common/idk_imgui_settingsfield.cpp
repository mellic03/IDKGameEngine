#include "idk_imgui_settingsfield.hpp"
#include <map>
#include <functional>


using SF = idk::SettingsField;


static char *sf_name = nullptr;


bool
idkImGui::InputSettingsField( const char *name, idk::SettingsField &field )
{
    static std::map<std::string, std::function<bool(SF&)>> InputT_table = {

        {"bool",  [](SF &sf){ return ImGui::Checkbox(sf_name, sf.getValue<bool>());       }},
        {"int",   [](SF &sf){ return ImGui::InputInt(sf_name, sf.getValue<int>());        }},
        {"float", [](SF &sf){ return ImGui::InputFloat(sf_name, sf.getValue<float>());    }}
    };

    static std::map<std::string, std::function<void(SF&)>> SliderT_table = {

        {"bool",  [](SF &sf){ return ImGui::Checkbox(sf_name, sf.getValue<bool>()); }},
        {"int",   [](SF &sf){ return ImGui::SliderInt(sf_name, sf.getValue<int>(), 0, 10); }},
        {"float", [](SF &sf){ return ImGui::SliderFloat(sf_name, sf.getValue<float>(), 0.0f, 10.0f); }}
    };

    static std::map<std::string, std::function<void(SF&)>> DragT_table = {

        {"bool",  [](SF &sf){ return ImGui::Checkbox(sf_name, sf.getValue<bool>());       }},
        {"int",   [](SF &sf){ return ImGui::InputInt(sf_name, sf.getValue<int>());        }},
        {"float", [](SF &sf){ return ImGui::InputFloat(sf_name, sf.getValue<float>());    }}
    };


    std::string cppname = std::string(name);
    sf_name = (char *)(cppname.c_str());
    std::string dtype = std::string(field.getType());

    return (InputT_table[dtype])(field);


    // if (dtype == "bool")
    // {
    //     ImGui::Checkbox(field.getName(), field.getValue<bool>());
    // }

    // else if (dtype == "int")
    // {
    //     ImGui::InputInt(field.getName(), field.getValue<int>());
    // }

    // else if (dtype == "float")
    // {
    //     ImGui::InputFloat(field.getName(), field.getValue<float>());
    // }


}
