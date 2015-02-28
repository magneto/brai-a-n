/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#using <System.dll>

#include "Models\Tree\DecisionTree.hpp"
#include "Models\Tree\Nodes\ANode.hpp"

using namespace System;
using namespace System::Collections::Generic;

public ref class DecisionTreeController {
	DecisionTreeController(DecisionTree ^tree);
	const Dictionary<String ^, ANode ^>	^getNodesList();
};