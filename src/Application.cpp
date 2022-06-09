#include "Nodes/Node.hpp"
#include "Nodes/InputNode.hpp"
#include "Nodes/OutputNode.hpp"
#include "Nodes/RasterizedNode.hpp"
#include "Application.hpp"
#include "Nodes/UUIDSingleton.hpp"
#include <SoftwareCore/DefaultLogger.hpp>
#include <SoftwareCore/Filesystem.hpp>
#include <yaml-cpp/yaml.h>
#include <imgui_node_editor.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <fstream>
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
    ed::NodeId InputNodeId;
    ed::NodeId OutputNodeId;
};

static ed::EditorContext* g_Context = nullptr;
static bool g_FirstFrame = true;
static ImVector<LinkInfo> g_Links;
static int g_NextLinkId = 100;

static std::string yamlFileNameSave;

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
    UUIDProvider.Get();
}

void Application_Load(const char* yamlFileName, bool exists)
{
    if (!exists)
    {
        yamlFileNameSave = yamlFileName;
        return;
    }

    ed::SetCurrentEditor(g_Context);

    yamlFileNameSave = yamlFileName;
    const auto rootNode = YAML::LoadFile(yamlFileName);
    const auto nodesNode = rootNode["nodes"];
    for (const auto& node : nodesNode)
    {
        const std::string type = node["type"].as<std::string>();

        const auto nodeMeta = node["meta"];

        const float posX = nodeMeta["x"].as<float>();
        const float posY = nodeMeta["y"].as<float>();

        const int nodeIndex = nodes.size();

        if (type == "output")
        {
            nodes.push_back(std::unique_ptr<Node>(new OutputNode{ posX , posY }));
        }
        else if (type == "input")
        {
            nodes.push_back(std::unique_ptr<Node>(new InputNode{ posX , posY }));
        }
        else if (type == "rasterized")
        {
            nodes.push_back(std::unique_ptr<Node>(new RasterizedNode{ posX , posY }));
        }

        nodes[nodeIndex]->Deserealize(node);
    }

    ed::SetCurrentEditor(nullptr);
}

void Application_Finalize()
{
    if (nodes.empty())
    {
        ed::DestroyEditor(g_Context);
        return;
    }

    ed::SetCurrentEditor(g_Context);

    std::ofstream ofs(yamlFileNameSave);

    YAML::Node rootNode;
    YAML::Node nodesNode = rootNode["nodes"];
    for (const auto& node : nodes)
    {
        const int nodeIndex = nodesNode.size();
        nodesNode.push_back(node->Serialize());
        // TODO: links -- rootNode["nodes"][rootNode["nodes"].size() - 1]
        
        const auto pos = node->GetPosition();
        nodesNode[nodeIndex]["meta"]["x"] = pos.x;
        nodesNode[nodeIndex]["meta"]["y"] = pos.y;
    }
    ofs << rootNode;
    ofs.close();

    ed::SetCurrentEditor(nullptr);

    ed::DestroyEditor(g_Context);
}

void Application_AddInputNode(const ImVec2& position)
{
    ed::SetCurrentEditor(g_Context);

    nodes.push_back(std::unique_ptr<Node>(new InputNode{ position.x, position.y }));
    nodes[nodes.size() - 1]->SetPosition(position);
}

void Application_AddOutputNode(const ImVec2& position)
{
    ed::SetCurrentEditor(g_Context);

    nodes.push_back(std::unique_ptr<Node>(new OutputNode{ position.x, position.y }));
    nodes[nodes.size() - 1]->SetPosition(position);
}

void Application_AddRasterizedNode(const ImVec2& position)
{
    ed::SetCurrentEditor(g_Context);

    nodes.push_back(std::unique_ptr<Node>(new RasterizedNode{ position.x, position.y }));
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
            if (inputPinId && outputPinId && inputPinId != outputPinId)
            {
                ed::NodeId inputPinNodeId = 0;
                ed::NodeId outputPinNodeId = 0;
                for (const auto& node : nodes)
                {
                    const int inputCount = node->GetInputCount();
                    const int outputCount = node->GetOutputCount();

                    for (int i = 0; i < inputCount; ++i)
                    {
                        const auto inputId = node->GetInputId(i);
                        if (inputId == inputPinId)
                        {
                            inputPinNodeId = node->GetId();
                            break;
                        }

                        if (inputId == outputPinId)
                        {
                            outputPinNodeId = node->GetId();
                            break;
                        }
                    }

                    for (int i = 0; i < outputCount; ++i)
                    {
                        const auto outputId = node->GetOutputId(i);
                        if (outputId == outputPinId)
                        {
                            outputPinNodeId = node->GetId();
                            break;
                        }

                        if (outputId == inputPinId)
                        {
                            inputPinNodeId = node->GetId();
                            break;
                        }
                    }
                }

                // TODO: Also make sure they are a valid pair by asking them.
                if (!inputPinNodeId || !outputPinNodeId)
                {
                    ed::RejectNewItem();
                }
                else
                {
                    if (ed::AcceptNewItem())
                    {
                        // Since we accepted new link, lets add one to our list of links.
                        g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId, inputPinNodeId, outputPinNodeId });

                        // Draw new link.
                        ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
                    }
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
        ed::NavigateToContent(0.8f);
    }

    ed::SetCurrentEditor(nullptr);

    g_FirstFrame = false;

    // ImGui::ShowMetricsWindow();
}

