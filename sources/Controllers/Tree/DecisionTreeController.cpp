/*
** Author: Sebastien Maire
** Contribs: Emmanuel Isidore
*/

#include "Controllers\Serialization\SerializationController.hpp"
#include "DecisionTreeController.hpp"

DecisionTreeController::DecisionTreeController() :
tree_(gcnew DecisionTree())
{}

void	DecisionTreeController::CheckTree() {
	if (!tree_)
		throw gcnew Exception("You must load a tree before you can do that.");
}

Dictionary<String^, ANode^> ^DecisionTreeController::getChildren(ANode^ Node) {
	CheckTree();
	return Node->getChildren();
}

void DecisionTreeController::addChild(ANode^ node, ANode ^child) {
	CheckTree();
	node->AddChild(child);
	tree_->RegisterNode(node);
}

List<ANode^> ^DecisionTreeController::getNodesList() {
	CheckTree();
	return nullptr;
}

Dictionary<String ^, Type^>^DecisionTreeController::getNodesTypes() {
	CheckTree();
	return nullptr;
}

void DecisionTreeController::setNodePos(ANode^ node, UInt32 x, UInt32 y) {
	CheckTree();
	node->setPosition(x, y);
}

void DecisionTreeController::Save(String ^path) {
	Serializer	^ser = gcnew Serializer();

	ser->Serialize(path, tree_);
}

void DecisionTreeController::Load(String ^path) {
	Serializer	^ser = gcnew Serializer();
	if (tree_) {
		throw gcnew Exception("Do you want to save before load another brain ?");
	}
	tree_ = ser->Unserialize(path);
}
