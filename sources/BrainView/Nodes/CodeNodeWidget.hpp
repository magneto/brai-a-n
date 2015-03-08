/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#pragma once

#include "BrainView\include.h"
//#include "BrainView.hpp"

ref class BrainView;

using namespace System::Collections::Generic;

#include "Models\Tree\Nodes\CodeNode.hpp"

//ref class NodeWidget;

#include "NodeWidget.hpp"

ref class CodeNodeWidget : NodeWidget<CodeNode ^>
{
public:
	TextBox		^tBox_;

public:

	CodeNodeWidget(BrainView ^curWin, int posX, int posY, String ^title, CodeNode ^node);

	// events
	void tBoxChanged(Object ^sender, TextChangedEventArgs ^e);
};

