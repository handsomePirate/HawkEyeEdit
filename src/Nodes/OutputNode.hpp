#pragma once
#include "Node.hpp"

class OutputNode : public Node
{
public:
	OutputNode(const float x, const float y);

	void Construct() override;

	int GetInputCount() const;
	int GetOutputCount() const;

	void Deserealize(const YAML::Node& node);
	YAML::Node Serialize() const;

	const char* GetType() const;

private:
	int inputCount_;
	std::vector<Resource> resources;
};
