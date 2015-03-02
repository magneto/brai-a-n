#include "NodeWidget.h"

#include "BrainView.h"
#include "Models\Tree\Nodes\CodeNode.hpp"


BrainView::BrainView(void) :
canvas_(gcnew Canvas()),
scroll_(gcnew ScrollViewer()){
	this->Title = "PFA";
	this->Width = 1200;
	this->Height = 800;
	this->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x20, 0x20, 0x20));

	this->MouseRightButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::RightClick);
	this->MouseUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::OnMouseClickWheelUp);
	this->MouseDown += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::OnMouseClickWheelDown);
	this->MouseMove += gcnew System::Windows::Input::MouseEventHandler(this, &BrainView::OnMouseMove);

	this->canvas_->Width = 10000;
	this->canvas_->Height = 10000;
	this->scroll_->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	this->scroll_->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	this->scroll_->Content = this->canvas_;


	this->Content = this->scroll_;
	NodeWidget ^a = gcnew NodeWidget(this, 100, 10, "test1", gcnew CodeNode("int b;"));
	/*NodeWidget ^b = gcnew NodeWidget(this, 210, 10, "test2");*/
}

Void BrainView::OnMouseClickWheelDown(Object^ sender, MouseButtonEventArgs^ e)
{
	if (e->ChangedButton == MouseButton::Middle)
	{
		this->moveWheel_ = true;
		this->lastWheelX_ = e->GetPosition(this).X;
		this->lastWheelY_ = e->GetPosition(this).Y;
	}
}

void	BrainView::OnMouseMove(Object ^sender, MouseEventArgs ^e) {
	if (moveWheel_)
	{
		scroll_->ScrollToHorizontalOffset(scroll_->ContentHorizontalOffset + (lastWheelX_ - e->GetPosition(this).X));
		scroll_->ScrollToVerticalOffset(scroll_->ContentVerticalOffset + (lastWheelY_ - e->GetPosition(this).Y));

	}
	lastWheelX_ = e->GetPosition(this).X;
	lastWheelY_ = e->GetPosition(this).Y;
}

Void BrainView::OnMouseClickWheelUp(Object^ sender, MouseButtonEventArgs^ e)
{
	if (e->ChangedButton == MouseButton::Middle)
		this->moveWheel_ = false;
}

Canvas ^BrainView::getCanvas() {
	return canvas_;
}

void BrainView::RightClick(Object^ sender, MouseButtonEventArgs^ e)
{
	if (menu_) {
		this->canvas_->Children->Remove(menu_);
	}
	menu_ = gcnew Menu();
	MenuItem^ i = gcnew MenuItem();
	i->Header = "Create Node";
	i->Click += gcnew System::Windows::RoutedEventHandler(this, &BrainView::NodesCreate);

	menu_->Items->Add(i);
	Canvas::SetTop(menu_, e->GetPosition(this).Y + this->scroll_->ContentVerticalOffset);
	Canvas::SetLeft(menu_, e->GetPosition(this).X + this->scroll_->ContentHorizontalOffset);
	this->canvas_->Children->Add(menu_);
}

void BrainView::NodesCreate(System::Object ^sender, System::Windows::RoutedEventArgs ^e)
{
	MenuItem ^i = (MenuItem ^)sender;

	CodeNode ^n = gcnew CodeNode("int a;");
	gcnew NodeWidget(this, 42 + this->scroll_->ContentHorizontalOffset, 42 + this->scroll_->ContentVerticalOffset, n->getName(), n);
	this->canvas_->Children->Remove(menu_);
}

void BrainView::OnMouseClickWin(Object^ sender, MouseButtonEventArgs^ e)
{
	if (menu_) {
		this->canvas_->Children->Remove(menu_);
	}
	if (selected_) {
		if (mode_ == BrainView::Mode::MOVE) {
			selected_->posX_ = (UInt32)e->GetPosition(this).X + this->scroll_->ContentHorizontalOffset;
			selected_->posY_ = (UInt32)e->GetPosition(this).Y + this->scroll_->ContentVerticalOffset;
			Canvas::SetTop(selected_->rootWidget_, e->GetPosition(this).Y + this->scroll_->ContentVerticalOffset);
			Canvas::SetLeft(selected_->rootWidget_, e->GetPosition(this).X + this->scroll_->ContentHorizontalOffset);
			selected_->recNode_->Stroke = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
			selected_ = nullptr;
		}
		else if (mode_ == BrainView::Mode::LINK_NODE) {
		}
	}
}

void	BrainView::UpdateLinks() {
	// loop over the nodes
	// redrawlines
}
