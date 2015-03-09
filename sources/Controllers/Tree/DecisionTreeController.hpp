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

public delegate ANode ^NodeCreationRoutine(TextBlock ^console);

public ref class DecisionTreeController {
public:
	DecisionTreeController();
	void	CheckTree();
	
public:
	DecisionTree ^tree_;
	System::Threading::Mutex	^treeMutex_; // to avoid running and loading at the same time
	// property Tree

public:
	Dictionary<ANode^, ANode^>					^getChildren(ANode^ node);
	void										addChild(ANode^ node, ANode^ child);
	List<ANode^>								^getNodesList();
	Dictionary<String ^, NodeCreationRoutine ^>	^getNodesTypes();
	void										setNodePos(ANode^ node, UInt32 x, UInt32 y);
	generic<typename T>
	void	Run(T value);
	void	Save(String ^path);
	void	Load(String ^path);

	void	RemoveNode(ANode ^node);
	ANode	^CreateCodeNode(TextBlock ^console);
	//ANode	^CreateFANNNode(TextBlock ^console);
};