/*
** Author: Sebastien Maire
** Contribs:
*/

#include "ANode.hpp"

ANode::ANode() :
	children_(gcnew Dictionary<String ^, ANode ^>()),
	posX_(0),
	posY_(0),
	number(Counter::getNumber()),
	name_("Node" + number) {
}

void	ANode::setPosition(UInt32 x, UInt32 y) {
	posX_ = x;
	posY_ = y;
}
String ^ANode::getName() { return name_; }
Dictionary<String ^, ANode ^>	^ANode::getChildren() { return children_; }


bool	ANode::AddChild(ANode ^node) {
	if (children_->ContainsKey(node->getName())) {
		return false;
	}
	children_[node->getName()] = node;
	return true;
}

void ANode::RemoveChild(String ^name) {
	if (children_->ContainsKey(name)) {
		children_->Remove(name);
	}
}
