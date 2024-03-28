#include "node-editor.hpp"

static ImNodesContext *context = ImNodes::CreateContext();

static idk::Allocator<idk::EditorNode>              m_nodes;
static std::unordered_map<int, std::pair<int, int>> m_links;



static void
idk_NodeEditor_Object_Position( idk::EngineAPI &api )
{

}



static void
idk_NodeEditor_Object( idk::EngineAPI &api, idk::EditorNode &node, int &ID )
{
    ImNodes::BeginNode(ID++);

    ImGui::PushItemWidth(60.0f);
        ImGui::Button("Droppu", ImVec2(60.0f, 30.0f));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY"))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int obj_id = *reinterpret_cast<int *>(payload->Data);
                node.obj_id = obj_id;
            }
        
            ImGui::EndDragDropTarget();
        }
    ImGui::PopItemWidth();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("int %d", node.obj_id);
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}


static void
idk_NodeEditor_Position( idk::EngineAPI &api, idk::EditorNode &node, int &ID )
{
    ImNodes::BeginNode(ID++);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(node.name.c_str());
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("Object ID: %d", node.obj_id);
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("float X");
    ImNodes::EndOutputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("float Y");
    ImNodes::EndOutputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("float Z");
    ImNodes::EndOutputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("vec3 XYZ");
    ImNodes::EndOutputAttribute();


    ImNodes::EndNode();
}



static void
idk_NodeEditor_Script( idk::EngineAPI &api, idk::EditorNode &node, int &ID )
{
    ImNodes::BeginNode(ID++);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(node.name.c_str());
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("Subject ID");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("Target ID");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("bool cond");
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("int out");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}


static void
idk_NodeEditor_Number( idk::EngineAPI &api, idk::EditorNode &node, int &ID )
{
    ImNodes::BeginNode(ID++);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Number");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginStaticAttribute(ID++);
        ImGui::PushItemWidth(60.0f);
        ImGui::InputFloat("Value", &node.condvalue);
        ImGui::PopItemWidth();
    ImNodes::EndStaticAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("float out");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}


static void
idk_NodeEditor_Condition( idk::EngineAPI &api, idk::EditorNode &node, int &ID )
{
    ImNodes::BeginNode(ID++);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Condition");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("float a");
    ImNodes::EndInputAttribute();

    ImNodes::BeginInputAttribute(ID++);
    ImGui::Text("float b");
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(ID++);
    ImGui::Text("bool out");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}



static void
idk_NodeEditor_BeginLinks()
{
    for (auto &[id, pair]: m_links)
    {
        ImNodes::Link(id, pair.first, pair.second);
    }
}


static void
idk_NodeEditor_EndLinks()
{
    static int num_links = 0;

    int link_id, start, end;
    if (ImNodes::IsLinkCreated(&start, &end))
    {
        m_links[num_links] = std::make_pair(start, end);

        idk_printvalue(start);
        idk_printvalue(end);

        num_links += 1;
    }

    if (ImNodes::IsLinkDestroyed(&link_id))
    {
        m_links.erase(link_id);
    }
}



void
idk_NodeEditor( idk::EngineAPI &api )
{
    context->CurrentAttributeFlags = ImNodesAttributeFlags_EnableLinkDetachWithDragClick;

    static bool first = true;

    if (first)
    {
        m_nodes.create(idk::EditorNode(idk::NodeType::OBJECT,    "Object " ICON_FA_CUBE));
        m_nodes.create(idk::EditorNode(idk::NodeType::POSITION,  "Position " ICON_FA_CUBE));
        m_nodes.create(idk::EditorNode(idk::NodeType::SCRIPT,    "Script " ICON_FA_FILE));
        m_nodes.create(idk::EditorNode(idk::NodeType::NUMBER,    "Number"));
        m_nodes.create(idk::EditorNode(idk::NodeType::CONDITION, "Condition"));

        first = false;
    }



    ImNodes::BeginNodeEditor();
    idk_NodeEditor_BeginLinks();

    int ID = 1;

    for (auto &node: m_nodes)
    {
        switch (node.type)
        {
            using enum idk::NodeType;

            default: break;

            case OBJECT:    idk_NodeEditor_Object       (api, node, ID);    break;
            case POSITION:  idk_NodeEditor_Position     (api, node, ID);    break;
            case SCRIPT:    idk_NodeEditor_Script       (api, node, ID);    break;
            case NUMBER:    idk_NodeEditor_Number       (api, node, ID);    break;
            case CONDITION: idk_NodeEditor_Condition    (api, node, ID);    break;

        }
    }

    ImNodes::EndNodeEditor();
    idk_NodeEditor_EndLinks();

}
