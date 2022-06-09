#pragma once
#include "Node.hpp"

class InputNode : public Node
{
public:
	InputNode(const float x, const float y);

	void Construct() override;

	int GetInputCount() const;
	int OutputCount() const;

	void Deserealize(const YAML::Node& node);
	YAML::Node Serialize() const;

	const char* GetType() const;

private:
	int outputCount_;
};
