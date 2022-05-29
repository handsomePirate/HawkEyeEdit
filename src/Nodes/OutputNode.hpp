#pragma once
#include "Node.hpp"

class OutputNode : public Node
{
public:
	OutputNode(int id, int pinIdStart, int inputCount);

	void Construct() override;
private:
	int inputCount_;
};
