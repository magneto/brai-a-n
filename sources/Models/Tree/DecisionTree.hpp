/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#using <System.dll>

#include "Models\Tree\Nodes\ANode.hpp"

public ref class DecisionTree {
public:
	DecisionTree();

	ANode			^rootNode_;
	List<ANode ^>	^standaloneNodes_;
};