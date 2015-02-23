/*
** Author: Sebastien Maire
** Contribs:
*/

#include "ANode.hpp"

ANode::ANode() :
	childs_(gcnew Dictionary<String ^, ANode ^>()), number(42) {
}

void	ANode::AddChild(String ^name, ANode ^node) {
	childs_->Add(name, node);
}
