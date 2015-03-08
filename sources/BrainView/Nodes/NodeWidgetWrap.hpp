/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#pragma once

using namespace System::Runtime::InteropServices;
ref class ANode;

generic<typename T> where T : ANode
ref class NodeWidgetWrap {
public:
	T		node_;
public:
	NodeWidgetWrap(T node);
};