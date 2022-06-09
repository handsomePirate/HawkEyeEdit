#include "InputNode.hpp"
#include "UUIDSingleton.hpp"
#include <SoftwareCore/DefaultLogger.hpp>
#include <SoftwareCore/String.hpp>
#include <SoftwareCore/Input.hpp>
#include <string>

InputNode::InputNode(const float x, const float y)
    : Node(x, y), outputCount_(1)
{}

void InputNode::Construct()
{
    ax::NodeEditor::BeginNode(Node::id_);

    const auto nodeSize = ax::NodeEditor::GetNodeSize(Node::id_);
    const auto nodePos = ax::NodeEditor::GetNodePosition(Node::id_);

    ImGui::PushID(Node::idStr_.c_str());
    ImGui::Text(("Input " + Node::idStr_).c_str());
    
    ImGui::BeginGroup();

    /*ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();*/

    const int inputIntWidth = 70;
    ImGui::PushItemWidth(inputIntWidth);
    ImGui::SetCursorPosX(nodePos.x + nodeSize.x - inputIntWidth
        - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputInt("", &outputCount_, 1, 1);
    ImGui::PopItemWidth();
    outputCount_ = (std::max)(1, (std::min)(Node::maxInputs, outputCount_));

    while (resources.size() > outputCount_)
    {
        resources.pop_back();
    }
    while (resources.size() < outputCount_)
    {
        resources.emplace_back(Node::id_, -1, resources.size());
    }

    for (int i = 0; i < outputCount_; ++i)
    {
        resources[i].Construct(i);
    }
    
    ImGui::EndGroup();

    ImGui::PopID();
    ax::NodeEditor::EndNode();
}

int InputNode::GetInputCount() const
{
    return 0;
}

int InputNode::GetOutputCount() const
{
    return outputCount_;
}

void InputNode::Deserealize(const YAML::Node& node)
{
    const std::string type = node["type"].as<std::string>();
    if (type != GetType())
    {
        CoreLogError(DefaultLogger, "The type of an output node has to be \'output\'");
        return;
    }
    Node::id_ = Core::String::UUIDStringToNumber(node["id"].as<std::string>());
    Node::idStr_ = Core::String::NumberToUUIDString(id_.Get());
    outputCount_ = node["resource-count"].as<int>();

    if (node["resources"])
    {
        const auto resourcesNode = node["resources"];
        for (const auto& resourceNode : resourcesNode)
        {
            resources.emplace_back(Node::id_, -1, resources.size());
            resources[resources.size() - 1].Deserialize(resourceNode);
        }
    }
    outputCount_ = resources.size();
}

YAML::Node InputNode::Serialize() const
{
    YAML::Node node;
    node["id"] = Core::String::NumberToUUIDString(Node::id_.Get());
    node["type"] = GetType();
    node["resource-count"] = outputCount_;
    for (int i = 0; i < outputCount_; ++i)
    {
        node["resources"][i] = resources[i].Serialize();
    }
    return node;
}

const char* InputNode::GetType() const
{
    return "input";
}
