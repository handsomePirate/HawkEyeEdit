#include "OutputNode.hpp"
#include <string>

OutputNode::OutputNode(int id, int pinIDStart, int inputCount)
    : Node(id, pinIDStart), inputCount_(inputCount) {}

void OutputNode::Construct()
{
    ax::NodeEditor::PinId pinId;
    int currentPinId = pinIdStart_;

    ax::NodeEditor::BeginNode(id_);
    ImGui::Text("Output");
    
    ImGui::BeginGroup();

    for (int i = 0; i < inputCount_; ++i)
    {
        pinId = currentPinId++;
        ax::NodeEditor::BeginPin(pinId, ax::NodeEditor::PinKind::Input);
        ImGui::Text(("> Input " + std::to_string(i)).c_str());
        ax::NodeEditor::EndPin();
    }

    /*ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();*/
    
    ImGui::EndGroup();

    ax::NodeEditor::EndNode();
}
