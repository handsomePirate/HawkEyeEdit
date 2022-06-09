#pragma once
#include "Node.hpp"

Node::Node(const float x, const float y)
	: id_(UUIDProvider.Get())
{
	ax::NodeEditor::SetNodePosition(id_, { x, y });
	idStr_ = Core::String::NumberToUUIDString(id_.Get());
}

const ax::NodeEditor::NodeId& Node::GetId() const
{
	return id_;
}

void Node::SetPosition(const ImVec2& position)
{
	ax::NodeEditor::SetNodePosition(id_, position);
}

ImVec2 Node::GetPosition() const
{
	return ax::NodeEditor::GetNodePosition(id_);
}

const ax::NodeEditor::PinId& Node::GetInputId(int inputPosition) const
{
	return id_.Get() + inputPosition + 1;
}

const ax::NodeEditor::PinId& Node::GetOutputId(int outputPosition) const
{
	return id_.Get() + maxInputs + outputPosition + 1;
}

ResourcePin::ResourcePin(ax::NodeEditor::NodeId nodeId, int pinSlot, bool isInput)
	: isInput_(isInput), dimensions_(), nodeId_(nodeId), pinSlot_(pinSlot)
{}

void ResourcePin::Construct()
{
	if (pinSlot_ == -1)
	{
		return;
	}

	std::string idSuffix;
	if (isInput_)
	{
		idSuffix = "-in";
	}
	else
	{
		idSuffix = "-out";
	}

	const auto nodeSize = ax::NodeEditor::GetNodeSize(nodeId_);
	const auto nodePos = ax::NodeEditor::GetNodePosition(nodeId_);

	std::string resourceText;
	if (isInput_)
	{
		resourceText = "<";
	}
	else
	{
		resourceText = ">";
	}

	const int offset = isInput_ ? pinSlot_ + 1 : Node::maxInputs + pinSlot_ + 1;

	if (!isInput_)
	{
		ImGui::SetCursorPosX(nodePos.x + nodeSize.x - ImGui::CalcTextSize(resourceText.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
	}

	const auto kind = isInput_ ? ax::NodeEditor::PinKind::Input : ax::NodeEditor::PinKind::Output;
	ax::NodeEditor::BeginPin(nodeId_.Get() + offset, kind);
	ImGui::Text(resourceText.c_str());
	ax::NodeEditor::EndPin();
}

bool ResourcePin::Exists() const
{
	return pinSlot_ != -1;
}

int ResourcePin::GetSlot() const
{
	return pinSlot_;
}

void ResourcePin::Deserialize(const YAML::Node& node)
{
	if (!node["dimensions"])
	{
		return;
	}
	if (node["dimensions"]["x"])
	{
		dimensions_.x = node["dimensions"]["x"].as<int>();
	}
	if (node["dimensions"]["y"])
	{
		dimensions_.y = node["dimensions"]["y"].as<int>();
	}
	if (node["dimensions"]["z"])
	{
		dimensions_.z = node["dimensions"]["z"].as<int>();
	}
	if (node["format"])
	{
		format_ = node["format"].as<std::string>();
	}
}

YAML::Node ResourcePin::Serialize() const
{
	YAML::Node node;
	node["dimensions"]["x"] = dimensions_.x;
	node["dimensions"]["y"] = dimensions_.y;
	node["dimensions"]["z"] = dimensions_.z;
	node["format"] = format_;
	return YAML::Node();
}

Resource::Resource(ax::NodeEditor::NodeId nodeId, int inPinSlot, int outPinSlot)
	: input_(nodeId, inPinSlot, true), output_(nodeId, outPinSlot, false),
	nodeIdStr_(Core::String::NumberToUUIDString(nodeId.Get()))
{}

void Resource::Construct()
{
	ImGui::PushID((nodeIdStr_ + "/" + std::to_string(input_.GetSlot()) + "/" + std::to_string(output_.GetSlot())).c_str());
	
	input_.Construct();
	if (input_.Exists())
	{
		ImGui::SameLine();
	}

	/*const int inputIntWidth = 70;
	ImGui::PushItemWidth(inputIntWidth);
	ImGui::Combo("type", &type_, typeOptions_);
	ImGui::PopItemWidth();*/

	if (output_.Exists())
	{
		ImGui::SameLine();
	}
	output_.Construct();
	
	ImGui::PopID();
}

void Resource::Deserialize(const YAML::Node& node)
{
	type_ = node["type"].as<int>();
	if (node["content-operation"])
	{
		contentOperation_ = node["content-operation"].as<std::string>();
	}
	input_.Deserialize(node["input"]);
	output_.Deserialize(node["output"]);
}

YAML::Node Resource::Serialize() const
{
	YAML::Node node;
	node["type"] = type_;
	node["content-operation"] = contentOperation_;
	node["input"] = input_.Serialize();
	node["output"] = output_.Serialize();
	return node;
}
