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
    ImGui::Text("Output");
    
    ImGui::BeginGroup();

    static Core::Input input;

    ImGui::PushItemWidth(70);
    ImGui::InputInt("", &inputCount_, 1, 1);
    ImGui::PopItemWidth();
    inputCount_ = (std::max)(1, (std::min)(maxInputs_, inputCount_));

    for (int i = 0; i < inputCount_; ++i)
    {
        ax::NodeEditor::BeginPin(GetInputId(i), ax::NodeEditor::PinKind::Input);
        ImGui::Text(("> Resource " + std::to_string(i + 1)).c_str());
        ax::NodeEditor::EndPin();
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

int OutputNode::OutputCount() const
{
    return 0;
}

void OutputNode::Deserealize(const YAML::Node& node)
{
    const std::string type = node["type"].as<std::string>();
    if (type != "output")
    {
        CoreLogError(DefaultLogger, "The type of an output node has to be \'output\'");
        return;
    }
    Node::id_ = Core::String::UUIDStringToNumber(node["id"].as<std::string>());
    Node::idStr_ = Core::String::NumberToUUIDString(id_.Get());
    inputCount_ = node["resource-count"].as<int>();
}

YAML::Node OutputNode::Serialize() const
{
    YAML::Node node;
    node["id"] = Core::String::NumberToUUIDString(Node::id_.Get());
    node["type"] = "output";
    node["resource-count"] = inputCount_;
    return node;
}

const char* OutputNode::GetType() const
{
    return "output";
}
