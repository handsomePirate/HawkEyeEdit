#pragma once
#include "UUIDSingleton.hpp"
#include <SoftwareCore/String.hpp>
#include <yaml-cpp/yaml.h>
#include <imgui_node_editor.h>
#include <vector>

class Node
{
public:
	Node(const float x, const float y) : id_(UUIDProvider.Get())
	{
		ax::NodeEditor::SetNodePosition(id_, { x, y });
		idStr_ = Core::String::NumberToUUIDString(id_.Get());
	}

	const ax::NodeEditor::NodeId& GetId() const { return id_; }
	void SetPosition(const ImVec2& position) { ax::NodeEditor::SetNodePosition(id_, position); }
	ImVec2 GetPosition() const { return ax::NodeEditor::GetNodePosition(id_); }

	virtual void Construct() = 0;

	virtual int GetInputCount() const = 0;
	virtual int OutputCount() const = 0;

	const ax::NodeEditor::PinId& GetInputId(int inputPosition) const { return id_.Get() + inputPosition + 1; }
	const ax::NodeEditor::PinId& GetOutputId(int outputPosition) const { return id_.Get() + maxInputs_ + outputPosition + 1; }

	virtual void Deserealize(const YAML::Node& node) = 0;
	virtual YAML::Node Serialize() const = 0;

	virtual const char* GetType() const = 0;

protected:
	ax::NodeEditor::NodeId id_;
	std::string idStr_;
	const int maxInputs_ = 20;
};
