/*
** Author: Sebastien Maire
** Contribs: Emmanuel Isidore
*/

#include "Controllers\Serialization\SerializationController.hpp"
#include "DecisionTreeController.hpp"
#include "Models\Tree\Nodes\CodeNode.hpp"
#include "Models\Tree\Nodes\FANNNode.hpp"

DecisionTreeController::DecisionTreeController() :
tree_(gcnew DecisionTree()),
treeMutex_(gcnew System::Threading::Mutex())
{}

void	DecisionTreeController::CheckTree() {
	if (!tree_)
		throw gcnew Exception("You must load a tree before you can do that.");
}

Dictionary<ANode ^, ANode^> ^DecisionTreeController::getChildren(ANode^ node) {
	CheckTree();
	return node->getChildren();
}

void DecisionTreeController::addChild(ANode^ node, ANode ^child) {
	CheckTree();
	node->AddChild(child);
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
	treeMutex_->WaitOne();
	Serializer	^ser = gcnew Serializer();

	ser->Serialize(path, tree_);
	treeMutex_->ReleaseMutex();
}

void DecisionTreeController::Load(String ^path) {
	treeMutex_->WaitOne();
	Console::WriteLine("Loada");
	Serializer	^ser = gcnew Serializer();
	if (tree_) {
		//throw gcnew Exception("Do you want to save before load another brain ?");
	}
	tree_ = nullptr;
	tree_ = ser->Unserialize(path);
	for each (ANode ^n in tree_->allNodes_) {
		n->Build();
	}
	treeMutex_->ReleaseMutex();
}

generic<typename T>
void	DecisionTreeController::Run(T value) {
	while (1) {
			treeMutex_->WaitOne();
		if (tree_)
		if (tree_->getRootNode())
			tree_->getRootNode()->Call(42);
		treeMutex_->ReleaseMutex();
	}
	
}

void	DecisionTreeController::RemoveNode(ANode ^node) {
	for each (ANode ^n in tree_->allNodes_) {
		n->getChildren()->Remove(node);
	}
	tree_->allNodes_->Remove(node);
}

ANode ^DecisionTreeController::CreateCodeNode(TextBlock ^console) {
	ANode ^n = gcnew CodeNode(console);

	tree_->RegisterNode(n);
	return n;
}
