#pragma once

#include "include.h"
#include <iostream>

ref class NodeWidget;

ref class BrainView : Window
{
public:
	enum class Mode {
		MOVE = 0,
		LINK_NODE = 1,
		UNLINK_NODE = 2,
		DELETE_LINK = 3,
		NONE = 4
	};
	Boolean moveWheel_;
	Mode	mode_;
	NodeWidget ^selected_;
	System::Windows::Controls::ContextMenu	^addNodeMenu_;
	Menu^ fileMenu_;
	Canvas	^canvas_;
	ScrollViewer^ scroll_;
	DockPanel^ dpWin_;
	int lastWheelX_;
	int lastWheelY_;
	TextBlock^ consoleDebug_;
	TextBlock ^consoleDebugTitle_;
	Grid ^consoleGrid_;

public:


	Canvas ^getCanvas();
	BrainView();
	void BrainView::OnMouseClickWin(Object^ sender, MouseButtonEventArgs^ e);
	void RightClick(Object ^sender, MouseButtonEventArgs^ e);
	void NodesCreate(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	Void OnMouseClickWheelUp(Object^ sender, MouseButtonEventArgs^ e);
	Void OnMouseClickWheelDown(Object^ sender, MouseButtonEventArgs^ e);
	void OnMouseMove(Object ^sender, MouseEventArgs ^e);
	void OnMouseClickSave(Object ^sender, RoutedEventArgs^ e);
	void OnMouseClickLoad(Object^ sender, RoutedEventArgs^ e);
	void WinSizeChanged(Object^ sender, SizeChangedEventArgs^ e);

	void UpdateLinks();
};
