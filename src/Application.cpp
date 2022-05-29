#include "Nodes/Node.hpp"
#include "Nodes/OutputNode.hpp"
#include "Application.hpp"
#include <imgui_node_editor.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <vector>
#include <memory>

namespace ed = ax::NodeEditor;

// Struct to hold basic information about connection between
// pins. Note that connection (aka. link) has its own ID.
// This is useful later with dealing with selections, deletion
// or other operations.
struct LinkInfo
{
    ed::LinkId Id;
    ed::PinId InputId;
    ed::PinId OutputId;
};

static ed::EditorContext* g_Context = nullptr;
static bool g_FirstFrame = true;
static ImVector<LinkInfo> g_Links;
static int g_NextLinkId = 100;

int uuid = 1;
std::vector<std::unique_ptr<Node>> nodes;

const char* Application_GetName()
{
    return "HawkEyeEdit";
}

void Application_Initialize()
{
    ed::Config config;
    config.SettingsFile = "HawkEyeEdit.json";
    g_Context = ed::CreateEditor(&config);
}

void Application_Finalize()
{
    ed::DestroyEditor(g_Context);
}

void Application_AddOutputNode(const ImVec2& position)
{
    ed::SetCurrentEditor(g_Context);

    nodes.push_back(std::unique_ptr<Node>(new OutputNode{ uuid++, uuid++, 2 }));
    nodes[nodes.size() - 1]->SetPosition(position);
}

void Application_Frame()
{
    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ed::SetCurrentEditor(g_Context);

    // Start interaction with editor.
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

    for (auto&& node : nodes)
    {
        node->Construct();
    }

    // Submit Links
    for (auto& linkInfo : g_Links)
    {
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
    }

    // Handle creation action, returns true if editor want to create new object (node or link)
    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            if (inputPinId && outputPinId) // both are valid, let's accept link
            {
                // TODO: Validate link can be created (avoid self-links).
                // ed::RejectNewItem()

                if (ed::AcceptNewItem())
                {
                    // Since we accepted new link, lets add one to our list of links.
                    g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

                    // Draw new link.
                    ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
                }
            }
        }
    }
    ed::EndCreate();


    // Handle deletion action
    if (ed::BeginDelete())
    {
        // There may be many links marked for deletion, let's loop over them.
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            // If you agree that link can be deleted, accept deletion.
            if (ed::AcceptDeletedItem())
            {
                // Then remove link from your data.
                for (auto& link : g_Links)
                {
                    if (link.Id == deletedLinkId)
                    {
                        g_Links.erase(&link);
                        break;
                    }
                }
            }

            // You may reject link deletion by calling:
            // ed::RejectDeletedItem();
        }
    }
    ed::EndDelete(); // Wrap up deletion action



    // End of interaction with editor.
    ed::End();

    if (g_FirstFrame)
    {
        ed::NavigateToContent(0.0f);
    }

    ed::SetCurrentEditor(nullptr);

    g_FirstFrame = false;

    // ImGui::ShowMetricsWindow();
}

