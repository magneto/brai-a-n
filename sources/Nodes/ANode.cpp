/*
** Author: Sebastien Maire
** Contribs:
*/

#include "ANode.hpp"

void	ANode::AddChild(String ^name, ANode ^node) {
	childs_->Add(name, node);
}
