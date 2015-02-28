/*
** Author: Sebastien Maire
** Contribs:
*/

#include "DecisionTree.hpp"

DecisionTree::DecisionTree() :
	rootNode_(nullptr), standaloneNodes_(gcnew List<ANode ^>(42)) {
}

List<ANode ^>	^DecisionTree::getNodes() {
	return allNodes_;
}