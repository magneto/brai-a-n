/*
** Author: Sebastien Maire
** Contribs:
** Description: Implementation of DecisionTree
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

