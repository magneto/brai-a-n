/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#using <System.dll>
#include "BrainView\include.h"
#include "Models\Tree\Nodes\ANode.hpp"

[SerializableAttribute]
public ref class DecisionTree {
public:
	ANode			^rootNode_;
	List<ANode ^>	^standaloneNodes_;
	List<ANode ^>	^allNodes_;
public:
	DecisionTree();


	ANode			^getRootNode();
	void			setRootNode(ANode ^node);
	List<ANode ^>	^getNodes();
	void			RegisterNode(ANode ^node);
};
