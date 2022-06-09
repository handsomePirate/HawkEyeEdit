#include "OutputNode.hpp"
#include "UUIDSingleton.hpp"
#include <SoftwareCore/DefaultLogger.hpp>
#include <SoftwareCore/String.hpp>
#include <SoftwareCore/Input.hpp>
#include <string>

OutputNode::OutputNode(const float x, const float y)
    : Node(x, y), inputCount_(1) 
{}

void OutputNode::Construct()
{
    ax::NodeEditor::BeginNode(id_);
    ImGui::PushID(Node::idStr_.c_str());
    ImGui::Text(("Output " + Node::idStr_).c_str());
    
    ImGui::BeginGroup();

    ImGui::PushItemWidth(70);
    ImGui::InputInt("", &inputCount_, 1, 1);
    ImGui::PopItemWidth();
    inputCount_ = (std::max)(1, (std::min)(Node::maxInputs, inputCount_));

    while (resources.size() > inputCount_)
    {
        resources.pop_back();
    }
    while (resources.size() < inputCount_)
    {
        resources.emplace_back(Node::id_, resources.size(), -1);
    }

    for (int i = 0; i < inputCount_; ++i)
    {
        resources[i].Construct(i);
        /*const std::string resourceText = "> Resource " + std::to_string(i + 1);
        ax::NodeEditor::BeginPin(GetInputId(i), ax::NodeEditor::PinKind::Input);
        ImGui::Text(resourceText.c_str());
        ax::NodeEditor::EndPin();*/
    }

    /*ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();*/
    
    ImGui::EndGroup();

    ImGui::PopID();
    ax::NodeEditor::EndNode();
}

int OutputNode::GetInputCount() const
{
    return inputCount_;
}

int OutputNode::GetOutputCount() const
{
    return 0;
}

void OutputNode::Deserealize(const YAML::Node& node)
{
    const std::string type = node["type"].as<std::string>();
    if (type != GetType())
    {
        CoreLogError(DefaultLogger, "The type of an output node has to be \'output\'");
        return;
    }
    Node::id_ = Core::String::UUIDStringToNumber(node["id"].as<std::string>());
    Node::idStr_ = Core::String::NumberToUUIDString(id_.Get());

    if (node["resources"])
    {
        const auto resourcesNode = node["resources"];
        for (const auto& resourceNode : resourcesNode)
        {
            resources.emplace_back(Node::id_, resources.size(), -1);
            resources[resources.size() - 1].Deserialize(resourceNode);
        }
    }
    inputCount_ = resources.size();
}

YAML::Node OutputNode::Serialize() const
{
    YAML::Node node;
    node["id"] = Core::String::NumberToUUIDString(Node::id_.Get());
    node["type"] = GetType();
    for (int i = 0; i < inputCount_; ++i)
    {
        node["resources"][i] = resources[i].Serialize();
    }
    return node;
}

const char* OutputNode::GetType() const
{
    return "output";
}
