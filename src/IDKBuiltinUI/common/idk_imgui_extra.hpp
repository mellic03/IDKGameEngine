#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include <idk_imgui/imgui.hpp>
#include <idk_imgui/imguizmo.hpp>
#include <idk_imgui/imnodes.hpp>

namespace EditorUI
{
    void dragVec3( std::string name, float *data, float min, float max,
                   float speed, const char *format, float default_value );

};


namespace idkImGui
{
    bool fileSelectPopup( const std::string &name, bool &open,
                          const std::string &entry_path, std::string &selection );


    bool RadioButton( const std::string &label_a, const std::string &label_b, bool &b );


    bool splitWindow_begin( const std::string &label );
    void splitWindow_split();
    void splitWindow_end();

    bool splitWindowH_begin( const std::string &label );
    void splitWindowH_split();
    void splitWindowH_end();

};
