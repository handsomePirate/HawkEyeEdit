#pragma once
#include "UUIDSingleton.hpp"
#include <imgui_node_editor.h>
#include <vector>

class Node
{
public:
	Node() : id_(UUIDProvider.Get()) {}

	const ax::NodeEditor::NodeId& GetId() const { return id_; }
	void SetPosition(const ImVec2& position) { ax::NodeEditor::SetNodePosition(id_, position); }

	virtual void Construct() = 0;

	virtual int GetInputCount() const = 0;
	virtual int OutputCount() const = 0;

	const ax::NodeEditor::PinId& GetInputId(int inputPosition) const { return inputIds_[inputPosition]; }
	const ax::NodeEditor::PinId& GetOutputId(int outputPosition) const { return outputIds_[outputPosition]; }

protected:
	ax::NodeEditor::NodeId id_;
	std::vector<ax::NodeEditor::PinId> inputIds_;
	std::vector<ax::NodeEditor::PinId> outputIds_;
};
