/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#include "BrainView\include.h"
//#using <System.dll>
//#include "Models\Tree\Nodes\ANode.hpp"
#include "NodeWidgetWrap.hpp"

using namespace System::Collections::Generic;

ref class ANode;
ref class BrainView;

generic<typename T> where T : ANode
ref class NodeWidget : NodeWidgetWrap<T> {
public:
	Dictionary<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^>	^parents_;
	Dictionary<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^>	^children_;

	UInt32			x_;
	UInt32			y_;
	property UInt32		posX_ {
		UInt32 get();
		void set(UInt32 i);
	}
	property UInt32		posY_ {
		UInt32 get();
		void set(UInt32 i);
	}
public:


	BrainView	^win;
	Grid		^rootWidget_;
	StackPanel	^spRoot_;
	Grid		^gTitle_;
	TextBox		^tBoxTitle_;
	StackPanel	^spButton_;
	Grid		^gButton_;
	Button		^bRemove_;
	Button		^bAdd_;
	Button		^bBuild_;
	Button		^bMove_;
	CheckBox	^boxRootNode_;
	ContextMenu	^menuNode_;
	Rectangle	^recNode_;

	Point	CalcLeftAttach();
	Point	CalcRightAttach();

	void	LinkChild(NodeWidget<T > ^parent);

	NodeWidget(BrainView ^curWin, int posX, int posY, String ^title, T node);

	// events
	void tBoxTitleChanged(Object ^sender, TextChangedEventArgs ^e);
	void ButtonBuild(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void OnMouseClickButtonMove(Object^ sender, MouseButtonEventArgs^ e);
	void NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e);
	void OnMouseMove(Object ^sender, MouseEventArgs ^e);
	void OpenMenuNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void AddLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void DeleteLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void OnMouseClickButtonRemove(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void CheckRootNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e);
	void UncheckRootNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e);

};