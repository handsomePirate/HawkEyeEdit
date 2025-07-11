#pragma once
#include "Node.hpp"
#include <vector>

class InputNode : public Node
{
public:
	InputNode(const float x, const float y);

	void Construct() override;

	int GetInputCount() const;
	int GetOutputCount() const;

	void Deserealize(const YAML::Node& node);
	YAML::Node Serialize() const;

	const char* GetType() const;

private:
	int outputCount_;
	std::vector<Resource> resources;
};
