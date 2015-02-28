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


void	ANode::AddChild(ANode ^node) {
	children_->Add(node->getName(), node);
}
