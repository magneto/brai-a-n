/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#pragma once

#include "include.h"
#include "Controllers\Tree\DecisionTreeController.hpp"
#include "Nodes\NodeWidget.hpp"

ref class BrainView : Window
{
	void	createNodesMenu_();
public:
	enum class Mode {
		MOVE = 0,
		LINK_NODE = 1,
		UNLINK_NODE = 2,
		DELETE_LINK = 3,
		NONE = 4
	};
	Boolean		moveWheel_;
	Mode		mode_;
	Object		^selected_;
	System::Windows::Controls::ContextMenu	^addNodeMenu_;
	Menu^			fileMenu_;
	Canvas			^canvas_;
	ScrollViewer	^scroll_;
	DockPanel		^dpWin_;
	int				lastWheelX_;
	int				lastWheelY_;
	TextBlock		^consoleDebug_;
	TextBlock		^consoleDebugTitle_;
	Grid			^consoleGrid_;
	DecisionTreeController	^treeController_;

public:
	Canvas ^getCanvas();
	BrainView();

	// Events
	void OnMouseClickWin(Object^ sender, MouseButtonEventArgs^ e);
	void RightClick(Object ^sender, MouseButtonEventArgs^ e);
	void NodesCreate(Object ^sender, System::Windows::RoutedEventArgs ^e);
	void OnMouseClickWheelUp(Object^ sender, MouseButtonEventArgs^ e);
	void OnMouseClickWheelDown(Object^ sender, MouseButtonEventArgs^ e);
	void OnMouseMove(Object ^sender, MouseEventArgs ^e);
	void OnMouseClickSave(Object ^sender, RoutedEventArgs^ e);
	void OnMouseClickLoad(Object^ sender, RoutedEventArgs^ e);
	void WinSizeChanged(Object^ sender, SizeChangedEventArgs^ e);

	// Tools functions
	void	DrawCanvas();

	NodeWidget<ANode ^>	^CreateCodeNodeWidget();
};

delegate NodeWidget<ANode ^>	^NodeWidgetCreateRout();
