#pragma once

#ifndef NODE_H_
#define NODE_H_

#include "include.h"
#include "BrainView.h"

ref class CodeNode;

ref class NodeWidget
{
public:
	CodeNode	^node_;
	UInt32		posX_;
	UInt32		posY_;
	BrainView^ win;
	Grid	^rootWidget_;
	StackPanel^ spRoot_;
	StackPanel^ spButton_;
	Grid^ gTitle_;
	Grid^ gButton_;
	TextBox^ tBoxTitle_;
	Rectangle^ recNode_;
	Button^ bRemove_;
	Button^ bAdd_;
	Button^ bBuild_;
	Button	^bMove_;
	TextBox^ tBox_;

public:
	NodeWidget(BrainView ^curWin, int posX, int posY, String ^title, CodeNode ^node);
	void AddLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void ButtonBuild(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void OnMouseClickButtonMove(Object^ sender, RoutedEventArgs^ e);
	void NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e);
	void	OnMouseMove(Object ^sender, MouseEventArgs ^e);

};

#endif NODE_H_ // NODE_H_
