/*
** Author: Sebastien Maire
** Contribs:
*/

#include "DecisionTree.hpp"
#include "Nodes\CodeNode.hpp"

DecisionTree::DecisionTree() :
rootNode_(nullptr),
standaloneNodes_(gcnew List<ANode ^>(42)),
allNodes_(gcnew List<ANode ^>(42)) {
}

void	DecisionTree::setRootNode(ANode ^node) { rootNode_ = node; }
ANode ^DecisionTree::getRootNode() { return rootNode_; }
List<ANode ^>	^DecisionTree::getNodes() { return allNodes_; }

void DecisionTree::RegisterNode(ANode ^node) {
	allNodes_->Add(node);
}

ANode ^DecisionTree::CreateCodeNode(TextBlock ^console) {
	ANode ^n = gcnew CodeNode(console);

	Console::WriteLine("ADDED");
	allNodes_->Add(n);
	return n;
}