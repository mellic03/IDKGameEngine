#include "idk_imgui_helpmarker.hpp"


void
idkImGui::HelpMarker( const char* desc )
{
    ImGui::TextDisabled("(?)");

    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}