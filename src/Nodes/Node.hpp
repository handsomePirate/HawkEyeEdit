#pragma once
#include <imgui_node_editor.h>
#include <vector>

class Node
{
public:
	Node(int id, int pinIdStart) : id_(id), pinIdStart_(pinIdStart) {}

	const ax::NodeEditor::NodeId& GetID() const { return id_; }
	void SetPosition(const ImVec2& position) { ax::NodeEditor::SetNodePosition(id_, position); }

	virtual void Construct() = 0;
protected:
	ax::NodeEditor::NodeId id_;
	int pinIdStart_;
	std::vector<ax::NodeEditor::PinId> pinIds_;
};
