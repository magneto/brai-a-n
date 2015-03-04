/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#pragma once

#include "include.h"
#include "BrainView.h"

using namespace System::Collections::Generic;

ref class CodeNode;

ref class NodeWidget
{
public:
	CodeNode	^node_;
		UInt32 x_;
	UInt32 y_;
	property UInt32		posX_ {
		UInt32 get() {
			return x_;
		}
		void set(UInt32 i) {
			x_ = i;
			Point childOffset = this->CalcLeftAttach();
			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^k in parents_)
				k->Value->Item2->X2 = i + childOffset.X;
						Point parent = win->selected_->CalcRightAttach();
			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^k in children_)
				k->Value->Item2->X1 = i + + parent.X;
		}
	}
	property UInt32		posY_ {
		UInt32 get() {
			return y_;
		}
		void set(UInt32 i) {
			y_ = i;
						Point childOffset = this->CalcLeftAttach();
			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^k in parents_)
				k->Value->Item2->Y2 = i + childOffset.Y;
									Point parent = win->selected_->CalcRightAttach();
			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^k in children_)
				k->Value->Item2->Y1 = i + + parent.Y;
		}
	}
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
	ContextMenu^ menuNode_;
	Dictionary<String ^, Tuple<NodeWidget ^, Line ^> ^> ^parents_;
	Dictionary<String ^, Tuple<NodeWidget ^, Line ^> ^> ^children_;



public:
	// To knwo coord of binding points
	Point	CalcLeftAttach();
	Point	CalcRightAttach();
	NodeWidget(BrainView ^curWin, int posX, int posY, String ^title, CodeNode ^node);
	void ButtonBuild(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void OnMouseClickButtonMove(Object^ sender, MouseButtonEventArgs^ e);
	void NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e);
	void	OnMouseMove(Object ^sender, MouseEventArgs ^e);
	void OpenMenuNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void AddLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void DeleteLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
};

