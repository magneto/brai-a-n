/*
** Author: Sebastien Maire
** Contribs: Emmanuel Isidore
*/

#pragma once

#using <System.dll>
#include <utility>

#include "Models\Tree\DecisionTree.hpp"
#include "Models\Tree\Nodes\ANode.hpp"

using namespace System;
using namespace System::Collections::Generic;

public ref class DecisionTreeController {
	DecisionTreeController(DecisionTree ^tree);
	
private:
	DecisionTree ^tree_;

public:
	Dictionary<String^, ANode^> ^getChildren(ANode^ Node);
	void addChild(ANode^ Node, ANode^ Child);
	List<ANode^> ^getNodesList();
	Dictionary<String ^, Type^>^getNodesTypes();
	void setNodePos(ANode^ Node, UInt32 X, UInt32 Y);
};