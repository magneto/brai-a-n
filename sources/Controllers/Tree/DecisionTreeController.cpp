/*
** Author: Sebastien Maire
** Contribs: Emmanuel Isidore
*/

#include "Controllers\Serialization\SerializationController.hpp"
#include "DecisionTreeController.hpp"
#include "Models\Tree\Nodes\CodeNode.hpp"
#include "Models\Tree\Nodes\FANNNode.hpp"

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
	for each (ANode ^n in tree_->allNodes_) {
		Console::WriteLine("=> {0}", n->getName());
	}
	return tree_->getNodes();
}

Dictionary<String ^, NodeCreationRoutine^>	^DecisionTreeController::getNodesTypes() {

	Dictionary < String ^, NodeCreationRoutine^ > ^d =
		gcnew Dictionary<String ^, NodeCreationRoutine ^>();
	
	d->Add("CodeNode", gcnew NodeCreationRoutine(this, &DecisionTreeController::CreateCodeNode));
	//d->Add("FANNNode", FANNNode::typeid);
	return d;
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
		//throw gcnew Exception("Do you want to save before load another brain ?");
	}
	tree_ = nullptr;
	tree_ = ser->Unserialize(path);
	//	for each (ANode ^n in tree_->getNodes()) {
	//	Console::WriteLine("LOAD {0}", n->getName());
	//}
}

ANode ^DecisionTreeController::CreateCodeNode(TextBlock ^console) {
	ANode ^n = gcnew CodeNode(console);

	tree_->RegisterNode(n);
	return n;
}
