#include "RasterizedNode.hpp"
#include "UUIDSingleton.hpp"
#include <SoftwareCore/DefaultLogger.hpp>
#include <SoftwareCore/String.hpp>
#include <SoftwareCore/Input.hpp>
#include <string>

RasterizedNode::RasterizedNode(const float x, const float y)
    : Node(x, y), inputCount_(1), outputCount_(1)
{}

void RasterizedNode::Construct()
{
    ax::NodeEditor::BeginNode(Node::id_);

    const auto nodeSize = ax::NodeEditor::GetNodeSize(Node::id_);
    const auto nodePos = ax::NodeEditor::GetNodePosition(Node::id_);

    ImGui::PushID((Node::idStr_ + "-in").c_str());
    ImGui::Text(("Rasterized " + Node::idStr_).c_str());

    ImGui::BeginGroup();

    ImGui::PushItemWidth(70);
    ImGui::InputInt("", &inputCount_, 1, 1);
    ImGui::PopItemWidth();
    inputCount_ = (std::max)(1, (std::min)(Node::maxInputs, inputCount_));

    for (int i = 0; i < inputCount_; ++i)
    {
        const std::string resourceText = "> Resource " + std::to_string(i + 1);
        ax::NodeEditor::BeginPin(GetInputId(i), ax::NodeEditor::PinKind::Input);
        ImGui::Text(resourceText.c_str());
        ax::NodeEditor::EndPin();
    }

    ImGui::PopID();

    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::PushID((Node::idStr_ + "-out").c_str());

    const int inputIntWidth = 70;
    ImGui::PushItemWidth(inputIntWidth);
    ImGui::SetCursorPosX(nodePos.x + nodeSize.x - inputIntWidth
        - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputInt("", &outputCount_, 1, 1);
    ImGui::PopItemWidth();
    outputCount_ = (std::max)(1, (std::min)(Node::maxInputs, outputCount_));

    for (int i = 0; i < outputCount_; ++i)
    {
        const std::string resourceText = "Resource " + std::to_string(i + 1) + " <";
        ImGui::SetCursorPosX(nodePos.x + nodeSize.x - ImGui::CalcTextSize(resourceText.c_str()).x
            - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ax::NodeEditor::BeginPin(GetOutputId(i), ax::NodeEditor::PinKind::Output);
        ImGui::Text(resourceText.c_str());
        ax::NodeEditor::EndPin();
    }

    ImGui::EndGroup();

    ImGui::PopID();
    ax::NodeEditor::EndNode();
}

int RasterizedNode::GetInputCount() const
{
    return inputCount_;
}

int RasterizedNode::GetOutputCount() const
{
    return outputCount_;
}

void RasterizedNode::Deserealize(const YAML::Node& node)
{
    const std::string type = node["type"].as<std::string>();
    if (type != GetType())
    {
        CoreLogError(DefaultLogger, "The type of an output node has to be \'output\'");
        return;
    }
    Node::id_ = Core::String::UUIDStringToNumber(node["id"].as<std::string>());
    Node::idStr_ = Core::String::NumberToUUIDString(id_.Get());
    inputCount_ = node["input"]["resource-count"].as<int>();
    outputCount_ = node["output"]["resource-count"].as<int>();
}

YAML::Node RasterizedNode::Serialize() const
{
    YAML::Node node;
    node["id"] = Core::String::NumberToUUIDString(Node::id_.Get());
    node["type"] = GetType();
    node["input"]["resource-count"] = inputCount_;
    node["output"]["resource-count"] = outputCount_;
    return node;
}

const char* RasterizedNode::GetType() const
{
    return "rasterized";
}
