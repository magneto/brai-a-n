/*
** Author: Sebastien Maire
** Contribs:
*/

#include "NodeWidget.hpp"
#include "BrainView\BrainView.hpp"
#include "BrainView\include.h"
#include "Models\Tree\Nodes\ANode.hpp"
//#using <PresentationCore.dll>

using namespace System::Windows::Media::Imaging;
using namespace System::Collections::Generic;

generic<typename T>
NodeWidget<T>::NodeWidget(BrainView ^curWin, int posX, int posY, String ^title, T node) :
	NodeWidgetWrap<T>(node),
	win(curWin),
	rootWidget_(gcnew Grid()),
	spRoot_(gcnew StackPanel()),
	spButton_(gcnew StackPanel()),
	gTitle_(gcnew Grid()),
	gButton_(gcnew Grid()),
	tBoxTitle_(gcnew TextBox()),
	recNode_(gcnew Rectangle()),
	bRemove_(gcnew Button()),
	bAdd_(gcnew Button()),
	bBuild_(gcnew Button()),
	bMove_(gcnew Button()),
	boxRootNode_(gcnew CheckBox()),
	x_(posX),
	y_(posY),
	parents_(gcnew Dictionary<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^>()),
	children_(gcnew Dictionary<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^>()),
	menuNode_(gcnew ContextMenu()) {
		rootWidget_->Width = 355;
		rootWidget_->Height = 305;

		spButton_->Orientation = Orientation::Horizontal;
		spRoot_->Width = 350;
		spRoot_->Height = 300;

		recNode_->Width = 352;
		recNode_->Height = 302;
		recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		recNode_->StrokeThickness = 2;


		gTitle_->Width = 350;
		gTitle_->Height = 30;

		tBoxTitle_->Text = title;
		tBoxTitle_->Foreground = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		tBoxTitle_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 100, 100, 100));
		tBoxTitle_->BorderThickness = System::Windows::Thickness(0, 0, 0, 1);
		tBoxTitle_->BorderBrush = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		tBoxTitle_->Text = node_->getName();
		tBoxTitle_->TextChanged += gcnew System::Windows::Controls::TextChangedEventHandler(this, &NodeWidget<T>::tBoxTitleChanged);
		gTitle_->Children->Add(tBoxTitle_);
		boxRootNode_->Content = "IsRootNode";
		boxRootNode_->Margin = System::Windows::Thickness(250, 5, 0, 0);
		boxRootNode_->Foreground = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		gTitle_->Children->Add(boxRootNode_);

		boxRootNode_->Checked += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::CheckRootNode);
		boxRootNode_->Unchecked += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::UncheckRootNode);
		spButton_->Width = 350;
		spButton_->Height = 70;
		spButton_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 100, 100, 100));
		bRemove_->Width = 50;
		bRemove_->Height = 50;
		bAdd_->Width = 50;
		bAdd_->Height = 50;
		bBuild_->Width = 50;
		bBuild_->Height = 50;
		bMove_->Width = 50;
		bMove_->Height = 50;
		bMove_->Tag = this;
		bRemove_->Margin = System::Windows::Thickness(29, 10, 0, 10);
		bRemove_->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::OnMouseClickButtonRemove);
		bAdd_->Margin = System::Windows::Thickness(29, 10, 0, 10);
		bBuild_->Margin = System::Windows::Thickness(29, 10, 0, 10);
		bMove_->Margin = System::Windows::Thickness(29, 10, 0, 10);

		bBuild_->Tag = this;
		bBuild_->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::ButtonBuild);
		bAdd_->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::OpenMenuNode);
		bMove_->PreviewMouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &NodeWidget<T>::OnMouseClickButtonMove);

		BitmapImage ^btm = gcnew BitmapImage(gcnew Uri(".\\Images\\objects_button_n.png", System::UriKind::Relative));
		Image ^img = gcnew Image();
		img->Source = btm;
		img->Stretch = Stretch::Fill;
		bAdd_->Content = img;
		BitmapImage ^btm2 = gcnew BitmapImage(gcnew Uri(".\\Images\\build_button_n.png", System::UriKind::Relative));
		Image ^img2 = gcnew Image();
		img2->Source = btm2;
		img2->Stretch = Stretch::Fill;
		bBuild_->Content = img2;
		BitmapImage ^btm3 = gcnew BitmapImage(gcnew Uri(".\\Images\\move_button_n.png", System::UriKind::Relative));
		Image ^img3 = gcnew Image();
		img3->Source = btm3;
		img3->Stretch = Stretch::Fill;
		bMove_->Content = img3;
		BitmapImage ^btm4 = gcnew BitmapImage(gcnew Uri(".\\Images\\remove_button_n.png", System::UriKind::Relative));
		Image ^img4 = gcnew Image();
		img4->Source = btm4;
		img4->Stretch = Stretch::Fill;
		bRemove_->Content = img4;


		spButton_->Children->Add(bMove_);
		spButton_->Children->Add(bRemove_);
		spButton_->Children->Add(bAdd_);
		spButton_->Children->Add(bBuild_);

		rootWidget_->Children->Add(spRoot_);
		rootWidget_->Children->Add(recNode_);
		spRoot_->Children->Add(gTitle_);
		spRoot_->Children->Add(spButton_);

		Canvas::SetTop(rootWidget_, posY);
		Canvas::SetLeft(rootWidget_, posX);
		win->getCanvas()->Children->Add(rootWidget_);

		win->scroll_->MouseMove += gcnew System::Windows::Input::MouseEventHandler(this, &NodeWidget<T>::OnMouseMove);
		win->scroll_->MouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(win, &BrainView::OnMouseClickWin);
		rootWidget_->MouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &NodeWidget<T>::NodeClic);

		MenuItem^ itemAdd = gcnew MenuItem();
		MenuItem^ itemDelete = gcnew MenuItem();
		itemAdd->Header = "Add child";
		itemDelete->Header = "Delete node";
		menuNode_->Items->Add(itemAdd);
		menuNode_->Items->Add(itemDelete);
		itemAdd->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::AddLink);
		itemDelete->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::DeleteLink);
}

generic<typename T>
void NodeWidget<T>::tBoxTitleChanged(Object ^sender, TextChangedEventArgs ^e) {
	node_->name_ = tBoxTitle_->Text;
}

generic<typename T>
void NodeWidget<T>::ButtonBuild(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	if (this->node_) {
		this->node_->Build();
	}
}

generic<typename T>
void NodeWidget<T>::OnMouseClickButtonMove(Object^ sender, MouseButtonEventArgs^ e)
{
	NodeWidget<T>	^sel = (NodeWidget<T> ^)win->selected_;

	if (sel) {
		sel->recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		win->selected_ = nullptr;
	}
	Button ^b = (Button ^)sender;

	NodeWidget<T> ^n = (NodeWidget<T> ^)b->Tag;
	n->recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0xEF, 0x57, 0x26));
	win->selected_ = n;
	win->mode_ = BrainView::Mode::MOVE;
}

generic<typename T>
void NodeWidget<T>::OnMouseClickButtonRemove(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	bool	lineRemoved = false;
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^w in this->parents_) {
		this->win->canvas_->Children->Remove(w->Value->Item2);
		if (w->Value->Item1->children_->ContainsKey(this->node_))
			w->Value->Item1->children_->Remove(this->node_);
	}

	lineRemoved = false;
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^w in this->children_) {
			this->win->canvas_->Children->Remove(w->Value->Item2);
			if (w->Value->Item1->parents_->ContainsKey(this->node_))
				w->Value->Item1->parents_->Remove(this->node_);
	}
	win->treeController_->RemoveNode(this->node_);
	this->win->canvas_->Children->Remove(this->rootWidget_);
}

generic<typename T>
void NodeWidget<T>::OpenMenuNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	menuNode_->IsOpen = true;	
}

generic<typename T>
void NodeWidget<T>::AddLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	win->selected_ = this;
	win->mode_ = BrainView::Mode::LINK_NODE;
}

generic<typename T>
void NodeWidget<T>::DeleteLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	win->selected_ = this;
	win->mode_ = BrainView::Mode::DELETE_LINK;

}

generic<typename T>
void	NodeWidget<T>::OnMouseMove(Object ^sender, MouseEventArgs ^e) {
	NodeWidget<T>	^sel = (NodeWidget<T> ^)win->selected_;

	if (sel) {
		if (win->mode_ == BrainView::Mode::MOVE) {
			sel->posX_ = (UInt32)e->GetPosition(win->canvas_).X;
			sel->posY_ = (UInt32)e->GetPosition(win->canvas_).Y;
			sel->node_->setPosition(sel->posX_, sel->posY_);
			Canvas::SetTop(sel->rootWidget_, e->GetPosition(win->canvas_).Y);
			Canvas::SetLeft(sel->rootWidget_, e->GetPosition(win->canvas_).X);
		}
	}

}

generic<typename T>
void	NodeWidget<T>::LinkChild(NodeWidget<T > ^parent) {
	NodeWidget<T>	^%sel = (NodeWidget<T> ^)win->selected_;

	if (!parents_->ContainsKey(this->node_) && !parent->children_->ContainsKey(node_)) {
		Line ^l = gcnew Line();

		l->Visibility = System::Windows::Visibility::Visible;
		l->StrokeThickness = 4;
		l->Stroke = System::Windows::Media::Brushes::Black;
		Point parentOffset = parent->CalcRightAttach();
		l->X1 = parent->posX_ + parentOffset.X;
		l->Y1 = parent->posY_ + parentOffset.Y;
		Point childOffset = this->CalcLeftAttach();
		l->X2 = childOffset.X + this->posX_;
		l->Y2 = childOffset.Y + this->posY_;

		this->parents_[parent->node_] =  gcnew Tuple<NodeWidget<T> ^, Line ^>(parent, l); // For access on both side to the line
		parent->children_[node_] =  gcnew Tuple<NodeWidget<T> ^, Line ^>(this, l);

		win->canvas_->Children->Add(l);
	}
}

generic<typename T>
void NodeWidget<T>::NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e)
{
	NodeWidget<T>	^%sel = (NodeWidget<T> ^)win->selected_;

	if (sel) {
		if (win->mode_ == BrainView::Mode::LINK_NODE) {
			if (!parents_->ContainsKey(this->node_) && !sel->children_->ContainsKey(node_)) {
				LinkChild(sel);

				this->win->treeController_->addChild(sel->node_, this->node_); // physically add child
			}
		}
		if (win->mode_ == BrainView::Mode::DELETE_LINK) {
			Line ^line = nullptr;


			if (sel->children_->ContainsKey(node_)) { // from parent to child
				win->canvas_->Children->Remove(sel->children_[node_]->Item2);
				sel->node_->RemoveChild(node_);
				sel->children_->Remove(node_);
				this->parents_->Remove(sel->node_);
			}

			if (this->children_->ContainsKey(sel->node_)) {
				win->canvas_->Children->Remove(this->children_[sel->node_]->Item2);
				this->node_->RemoveChild(sel->node_);
				sel->parents_->Remove(node_);
				this->children_->Remove(sel->node_);
			}
		}
		win->mode_ = BrainView::Mode::NONE;
		win->selected_ = nullptr;
	}
}
generic<typename T>
void NodeWidget<T>::CheckRootNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	ANode ^root = this->win->treeController_->tree_->getRootNode();

	if (!root) {
		this->win->treeController_->tree_->setRootNode(this->node_);
	}
	else {
		node_->console_->Text += String::Format("You must unset root node on node \"{0}\"\n", root->getName());
		boxRootNode_->Unchecked -= gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::UncheckRootNode);
		boxRootNode_->IsChecked = false;
		boxRootNode_->Unchecked += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget<T>::UncheckRootNode);
	}
}

generic<typename T>
void NodeWidget<T>::UncheckRootNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	this->win->treeController_->tree_->setRootNode(nullptr);
}

generic<typename T>
	UInt32	NodeWidget<T>::posX_::get() {
	return x_;
}

generic<typename T>
void	NodeWidget<T>::posX_::set(UInt32 i) {
	NodeWidget<T>	^%sel = (NodeWidget<T> ^)win->selected_;
	x_ = i;
	Point childOffset = this->CalcLeftAttach();
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^k in parents_)
		k->Value->Item2->X2 = i + childOffset.X;
	Point parent = sel->CalcRightAttach();
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^k in children_)
		k->Value->Item2->X1 = i + + parent.X;
}

generic<typename T>
UInt32	NodeWidget<T>::posY_::get() {
	return y_;
}

generic<typename T>
void	NodeWidget<T>::posY_::set(UInt32 i) {
	NodeWidget<T>	^%sel = (NodeWidget<T> ^)win->selected_;
	y_ = i;
	Point childOffset = this->CalcLeftAttach();
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^k in parents_)
		k->Value->Item2->Y2 = i + childOffset.Y;
	Point parent = sel->CalcRightAttach();
	for each (KeyValuePair<ANode ^, Tuple<NodeWidget<T> ^, Line ^> ^> ^k in children_)
		k->Value->Item2->Y1 = i + + parent.Y;
}

generic<typename T>
Point	NodeWidget<T>::CalcLeftAttach() {
	return Point(0, rootWidget_->Height / 2);
}

generic<typename T>
Point	NodeWidget<T>::CalcRightAttach() {
	return Point(rootWidget_->Width, rootWidget_->Height / 2);
}