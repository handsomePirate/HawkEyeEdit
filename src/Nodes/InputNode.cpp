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
    ax::NodeEditor::BeginNode(id_);
    ImGui::PushID(Node::idStr_.c_str());
    ImGui::Text("Input");
    
    ImGui::BeginGroup();
    
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();

    static Core::Input input;

    ImGui::PushItemWidth(70);
    ImGui::InputInt("", &outputCount_, 1, 1);
    ImGui::PopItemWidth();
    outputCount_ = (std::max)(1, (std::min)(maxInputs_, outputCount_));

    for (int i = 0; i < outputCount_; ++i)
    {
        ax::NodeEditor::BeginPin(GetOutputId(i), ax::NodeEditor::PinKind::Output);
        ImGui::Text(("Resource " + std::to_string(i + 1) + " <").c_str());
        ax::NodeEditor::EndPin();
    }
    
    ImGui::EndGroup();

    ImGui::PopID();
    ax::NodeEditor::EndNode();
}

int InputNode::GetInputCount() const
{
    return 0;
}

int InputNode::OutputCount() const
{
    return outputCount_;
}

void InputNode::Deserealize(const YAML::Node& node)
{
    const std::string type = node["type"].as<std::string>();
    if (type != "input")
    {
        CoreLogError(DefaultLogger, "The type of an output node has to be \'output\'");
        return;
    }
    Node::id_ = Core::String::UUIDStringToNumber(node["id"].as<std::string>());
    Node::idStr_ = Core::String::NumberToUUIDString(id_.Get());
    outputCount_ = node["resource-count"].as<int>();
}

YAML::Node InputNode::Serialize() const
{
    YAML::Node node;
    node["id"] = Core::String::NumberToUUIDString(Node::id_.Get());
    node["type"] = "input";
    node["resource-count"] = outputCount_;
    return node;
}

const char* InputNode::GetType() const
{
    return "input";
}
