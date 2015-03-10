/*
** Author: Sebastien Maire
** Contribs:
*/

#include "ANode.hpp"

ANode::ANode(TextBlock ^console) :
	console_(console),
	coreMutex_(gcnew System::Threading::Mutex()),
	children_(gcnew Dictionary<ANode ^, ANode ^>()),
	posX_(0),
	posY_(0),
	number(Counter::getNumber()),
	name_("Node" + number) {
}

void	ANode::setPosition(UInt32 x, UInt32 y) {
	posX_ = x;
	posY_ = y;
}

Tuple<UInt32, UInt32>	^ANode::getPosition() {
	return gcnew Tuple<UInt32, UInt32>(posX_, posY_);
}
String ^ANode::getName() { return name_; }
Dictionary<ANode ^, ANode ^>	^ANode::getChildren() { return children_; }


bool	ANode::AddChild(ANode ^node) {
	if (children_->ContainsKey(node)) {
		return false;
	}
	children_[node] = node;
	return true;
}

void ANode::RemoveChild(ANode ^node) {
	if (children_->ContainsKey(node)) {
		children_->Remove(node);
	}
}

generic<typename T>
void ANode::Call(T val) {
	Dictionary<String ^, ANode ^>	^namedC = gcnew Dictionary<String ^, ANode ^>();

	for each (KeyValuePair<ANode ^, ANode ^> ^c in children_)
	{
		namedC->Add(c->Key->getName(), c->Value);
	}
	this->Process(namedC, val);
}