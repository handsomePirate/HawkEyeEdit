#include "OutputNode.hpp"
#include "UUIDSingleton.hpp"
#include <SoftwareCore/DefaultLogger.hpp>
#include <string>

// TODO: UUID generator.

OutputNode::OutputNode()
    : Node(), inputCount_(1) {}

void OutputNode::Construct()
{
    const int maxInputs = 10;

    for (int i = 0; i < maxInputs; ++i)
    {
        inputIds_.push_back(UUIDProvider.Get());
    }

    ax::NodeEditor::BeginNode(id_);
    ImGui::Text("Output");
    
    ImGui::BeginGroup();

    ImGui::PushItemWidth(70);
    ImGui::InputInt("", &inputCount_, 1, 1);
    ImGui::PopItemWidth();
    inputCount_ = (std::max)(1, (std::min)(maxInputs, inputCount_));

    for (int i = 0; i < inputCount_; ++i)
    {
        ax::NodeEditor::BeginPin(inputIds_[i], ax::NodeEditor::PinKind::Input);
        ImGui::Text(("> Input " + std::to_string(i + 1)).c_str());
        ax::NodeEditor::EndPin();
    }

    /*ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();*/
    
    ImGui::EndGroup();

    ax::NodeEditor::EndNode();
}
