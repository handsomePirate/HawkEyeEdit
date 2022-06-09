#pragma once
#include "UUIDSingleton.hpp"
#include <SoftwareCore/String.hpp>
#include <yaml-cpp/yaml.h>
#include <imgui_node_editor.h>
#include <vector>

class Node
{
public:
	Node(const float x, const float y);

	const ax::NodeEditor::NodeId& GetId() const;
	void SetPosition(const ImVec2& position);
	ImVec2 GetPosition() const;

	virtual void Construct() = 0;

	virtual int GetInputCount() const = 0;
	virtual int GetOutputCount() const = 0;

	const ax::NodeEditor::PinId& GetInputId(int inputPosition) const;
	const ax::NodeEditor::PinId& GetOutputId(int outputPosition) const;

	virtual void Deserealize(const YAML::Node& node) = 0;
	virtual YAML::Node Serialize() const = 0;

	virtual const char* GetType() const = 0;

	static const int maxInputs = 20;

protected:
	ax::NodeEditor::NodeId id_;
	std::string idStr_;
};

struct Dimensions
{
	Dimensions() = default;
	int x, y, z;
};

class ResourcePin
{
public:
	ResourcePin(ax::NodeEditor::NodeId nodeId, int pinSlot, bool isInput);

	void Construct();

	bool Exists() const;
	int GetSlot() const;

	void Deserialize(const YAML::Node& node);
	YAML::Node Serialize() const;

private:
	ax::NodeEditor::NodeId nodeId_;
	std::string nodeIdStr_;
	int pinSlot_;
	Dimensions dimensions_;
	std::string format_;
	bool isInput_;
};

class Resource
{
public:
	Resource(ax::NodeEditor::NodeId nodeId, int inPinSlot, int outPinSlot);

	void Construct();

	void Deserialize(const YAML::Node& node);
	YAML::Node Serialize() const;

private:
	int type_ = 0;
	std::string contentOperation_;

	std::string nodeIdStr_;

	ResourcePin input_;
	ResourcePin output_;

	const char* typeOptions_ = "image\0buffer";
};
