#include "NodeWidget.h"
#include "Models\Tree\Nodes\CodeNode.hpp"

using namespace System::Windows::Media::Imaging;

NodeWidget::NodeWidget(BrainView ^curWin, int posX, int posY, String ^title, CodeNode ^node) :
	node_(node),
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
	tBox_(gcnew TextBox()),
	x_(posX),
	y_(posY),
	parents_(gcnew Dictionary<String ^, Tuple<NodeWidget ^, Line ^> ^>()),
	children_(gcnew Dictionary<String ^, Tuple<NodeWidget ^, Line ^> ^>()),
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
		gTitle_->Children->Add(tBoxTitle_);

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
		bAdd_->Margin = System::Windows::Thickness(29, 10, 0, 10);
		bBuild_->Margin = System::Windows::Thickness(29, 10, 0, 10);
		bMove_->Margin = System::Windows::Thickness(29, 10, 0, 10);

		bBuild_->Tag = this;
		bBuild_->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget::ButtonBuild);
		bAdd_->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget::OpenMenuNode);
		bMove_->PreviewMouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &NodeWidget::OnMouseClickButtonMove);


		BitmapImage ^btm = gcnew BitmapImage(gcnew Uri(".\\Images\\objects_button_n.png", System::UriKind::Relative));
		Image ^img = gcnew Image();
		img->Source = btm;
		img->Stretch = Stretch::Fill;
		bAdd_->Content = img;
		bRemove_->Content = img;
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

		spButton_->Children->Add(bMove_);
		spButton_->Children->Add(bRemove_);
		spButton_->Children->Add(bAdd_);
		spButton_->Children->Add(bBuild_);

		tBox_->Width = 350;
		tBox_->Height = 220;
		tBox_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 100, 100, 100));
		tBox_->BorderBrush = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		tBox_->BorderThickness = System::Windows::Thickness(0, 1, 0, 0);
		tBox_->Margin = System::Windows::Thickness(0, -1, 0, 0);
		tBox_->Foreground = gcnew SolidColorBrush(Colors::White);
		tBox_->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
		tBox_->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;


		rootWidget_->Children->Add(spRoot_);
		rootWidget_->Children->Add(recNode_);
		spRoot_->Children->Add(gTitle_);
		spRoot_->Children->Add(spButton_);
		spRoot_->Children->Add(tBox_);



		Canvas::SetTop(rootWidget_, posY);
		Canvas::SetLeft(rootWidget_, posX);
		win->getCanvas()->Children->Add(rootWidget_);

		win->scroll_->MouseMove += gcnew System::Windows::Input::MouseEventHandler(this, &NodeWidget::OnMouseMove);
		win->scroll_->MouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(win, &BrainView::OnMouseClickWin);
		rootWidget_->MouseLeftButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &NodeWidget::NodeClic);

		MenuItem^ itemAdd = gcnew MenuItem();
		MenuItem^ itemDelete = gcnew MenuItem();
		itemAdd->Header = "Add node";
		itemDelete->Header = "Delete node";
		menuNode_->Items->Add(itemAdd);
		menuNode_->Items->Add(itemDelete);
		itemAdd->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget::AddLink);
		itemDelete->Click += gcnew System::Windows::RoutedEventHandler(this, &NodeWidget::DeleteLink);
}

void NodeWidget::ButtonBuild(System::Object ^sender, System::Windows::RoutedEventArgs ^e) {
	if (this->node_) {
		this->node_->Build();
	}
}

void NodeWidget::OnMouseClickButtonMove(Object^ sender, MouseButtonEventArgs^ e)
{
	if (win->selected_) {
		win->selected_->recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		win->selected_ = nullptr;
	}
	Button ^b = (Button ^)sender;

	NodeWidget ^n = (NodeWidget ^)b->Tag;
	n->recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0xEF, 0x57, 0x26));
	win->selected_ = n;
	win->mode_ = BrainView::Mode::MOVE;
}

void NodeWidget::OpenMenuNode(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	menuNode_->IsOpen = true;	
}

void NodeWidget::AddLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	win->selected_ = this;
	win->mode_ = BrainView::Mode::LINK_NODE;
}

void NodeWidget::DeleteLink(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	win->selected_ = this;
	win->mode_ = BrainView::Mode::DELETE_LINK;
}

void	NodeWidget::OnMouseMove(Object ^sender, MouseEventArgs ^e) {
	if (win->selected_) {
		if (win->mode_ == BrainView::Mode::MOVE) {
			win->selected_->posX_ = (UInt32)e->GetPosition(win->canvas_).X;
			win->selected_->posY_ = (UInt32)e->GetPosition(win->canvas_).Y;
			Canvas::SetTop(win->selected_->rootWidget_, e->GetPosition(win->canvas_).Y);
			Canvas::SetLeft(win->selected_->rootWidget_, e->GetPosition(win->canvas_).X);
		}
	}
}

Point	NodeWidget::CalcLeftAttach() {
	return Point(0, rootWidget_->Height / 2);
}
Point	NodeWidget::CalcRightAttach() {
	return Point(rootWidget_->Width, rootWidget_->Height / 2);
}

void NodeWidget::NodeClic(System::Object ^sender, System::Windows::Input::MouseButtonEventArgs ^e)
{
	if (win->selected_) {
		if (win->mode_ == BrainView::Mode::LINK_NODE) {
			if (!parents_->ContainsKey(this->node_->getName()) && !win->selected_->children_->ContainsKey(node_->getName())) {
				Line ^l = gcnew Line();

				l->Visibility = System::Windows::Visibility::Visible;
				l->StrokeThickness = 4;
				l->Stroke = System::Windows::Media::Brushes::Black;
				Point parent = win->selected_->CalcRightAttach();
				l->X1 = win->selected_->posX_ + parent.X;
				l->Y1 = win->selected_->posY_ + parent.Y;
				Point childOffset = this->CalcLeftAttach();
				l->X2 = childOffset.X + this->posX_;
				l->Y2 = childOffset.Y + this->posY_;

				node_->AddChild(win->selected_->node_);

				this->parents_[win->selected_->node_->getName()] =  gcnew Tuple<NodeWidget ^, Line ^>(win->selected_, l);
				win->selected_->children_[node_->getName()] =  gcnew Tuple<NodeWidget ^, Line ^>(this, l);

				win->canvas_->Children->Add(l);
			}
		}
		if (win->mode_ == BrainView::Mode::DELETE_LINK) {
			Line ^line = nullptr;

			// from parent to child
			if (win->selected_->children_->ContainsKey(node_->getName())) {
				win->canvas_->Children->Remove(win->selected_->children_[node_->getName()]->Item2);
				win->selected_->node_->RemoveChild(node_->getName());
				win->selected_->children_->Remove(node_->getName());
				this->parents_->Remove(win->selected_->node_->getName());
			}

			if (this->children_->ContainsKey(win->selected_->node_->getName())) {
				win->canvas_->Children->Remove(this->children_[win->selected_->node_->getName()]->Item2);
				this->node_->RemoveChild(win->selected_->node_->getName());
				win->selected_->parents_->Remove(node_->getName());
				this->children_->Remove(win->selected_->node_->getName());
			}

			/*
			win->selected_->parents_->ContainsKey(node_->getName());

			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^parent in parents_) {
				if (parent->Value->Item1->children_->ContainsKey(curName)) {
					line = parent->Value->Item1->children_[curName]->Item2;
					parent->Value->Item1->
					parent->Value->Item1->children_->Remove(curName);

				}
			}

			for each (KeyValuePair<String ^, Tuple<NodeWidget ^, Line ^> ^> ^child in children_) {
				if (child->Value->Item1->parents_->ContainsKey(curName)) {
					line = child->Value->Item1->parents_[curName]->Item2;
					child->Value->Item1->parents_[curName]->Item1->node_->RemoveChild(curName);
					child->Value->Item1->parents_->Remove(this->node_->getName());

				}
			}

			if (line) {
				win->canvas_->Children->Remove(line);
			}*/

		}
		win->mode_ = BrainView::Mode::NONE;
		win->selected_ = nullptr;
	}
}
