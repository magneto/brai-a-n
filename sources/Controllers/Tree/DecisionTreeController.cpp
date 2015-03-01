/*
** Author: Sebastien Maire
** Contribs:Emmanuel Isidore
*/

#include "DecisionTreeController.hpp"

DecisionTreeController::DecisionTreeController(DecisionTree ^tree) :
tree_(tree)
{}
Dictionary<String^, ANode^> ^DecisionTreeController::getChildren(ANode^ Node)
{return Node->getChildren();}

void DecisionTreeController::addChild(ANode^ node, ANode ^child) {
	node->AddChild(child);
	tree_->RegisterNode(node);
}

List<ANode^> ^DecisionTreeController::getNodesList()
{return nullptr;}

Dictionary<String ^, Type^>^DecisionTreeController::getNodesTypes()
{return nullptr;}

void DecisionTreeController::setNodePos(ANode^ Node, UInt32 X, UInt32 Y)
{Node->setPosition(X, Y);}

