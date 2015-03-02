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
	UInt32 _x;
	UInt32 _y;
	property UInt32		posX_ {
		UInt32 get() {
			return _x;
		}
		void set(UInt32 i) {
			_x = i;
			for each (Line ^l in lines_)
				l->X1 = i;
		}
	}
	property UInt32		posY_ {
		UInt32 get() {
			return _y;
		}
		void set(UInt32 i) {
			_y = i;
			for each (Line ^l in lines_)
				l->Y1 = i;
		}
	}
	List<Line ^> ^lines_;
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
	void OnMouseClickButtonMove(Object^ sender, MouseButtonEventArgs^ e);
	void NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e);
	void	OnMouseMove(Object ^sender, MouseEventArgs ^e);

};

#endif NODE_H_ // NODE_H_
