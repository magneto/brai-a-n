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

void DecisionTreeController::addChild(ANode^ Node, ANode ^Child)
{Node->AddChild(Child);}

List<ANode^> ^DecisionTreeController::getNodesList()
{return nullptr;}

Dictionary<String ^, Type^>^DecisionTreeController::getNodesTypes()
{return nullptr;}

void DecisionTreeController::setNodePos(ANode^ Node, UInt32 X, UInt32 Y)
{Node->setPosition(X, Y);}

