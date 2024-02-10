#pragma once

#include <IDKGameEngine/IDKengine.hpp>
#include <idk_imgui/imgui.hpp>

#include <filesystem>

namespace idkImGui
{
    bool AssetBrowser( const char *name, std::filesystem::directory_entry &current_dir );

};