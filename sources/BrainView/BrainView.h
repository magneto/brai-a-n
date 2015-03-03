#pragma once

#ifndef FENETRE_H_
#define FENETRE_H_

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
	};
	Boolean moveWheel_;
	Mode	mode_;
	NodeWidget ^selected_;
	Menu	^menu_;
	Menu^ fileMenu_;
	Canvas	^canvas_;
	ScrollViewer^ scroll_;
	DockPanel^ dpWin_;
	int lastWheelX_;
	int lastWheelY_;

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

#endif // FENETRE_H_
