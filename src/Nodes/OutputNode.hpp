#pragma once
#include "Node.hpp"

class OutputNode : public Node
{
public:
	OutputNode();

	void Construct() override;
private:
	int inputCount_;
};
