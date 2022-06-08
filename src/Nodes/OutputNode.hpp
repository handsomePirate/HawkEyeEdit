#pragma once
#include "Node.hpp"

class OutputNode : public Node
{
public:
	OutputNode();

	void Construct() override;

	int GetInputCount() const;
	int OutputCount() const;
private:
	int inputCount_;
};
