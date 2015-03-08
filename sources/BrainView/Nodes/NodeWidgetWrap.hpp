/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#pragma once

ref class ANode;

generic<typename T> where T : ANode
ref class NodeWidgetWrap {
public:
	T		node_;
public:
	NodeWidgetWrap(T node);
};