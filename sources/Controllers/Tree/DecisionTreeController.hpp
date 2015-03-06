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
public:
	DecisionTreeController();
	void	CheckTree();
	
private:
	DecisionTree ^tree_;

public:
	Dictionary<String^, ANode^> ^getChildren(ANode^ node);
	void addChild(ANode^ node, ANode^ child);
	List<ANode^> ^getNodesList();
	Dictionary<String ^, NodeCreationRoutine ^>^getNodesTypes();
	void setNodePos(ANode^ node, UInt32 x, UInt32 y);
	void	Save(String ^path);
	void	Load(String ^path);
};