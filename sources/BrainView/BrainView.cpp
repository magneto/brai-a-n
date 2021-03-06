/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#using <System.Windows.Forms.dll>

#include "Nodes\CodeNodeWidget.hpp"
#include "BrainView.hpp"

using namespace System::Windows::Media::Imaging;
using namespace Microsoft::Win32;

void	BrainView::createNodesMenu_() {
	Dictionary<String ^, NodeWidgetCreateRout ^>	createRouts;

	createRouts.Add("CodeNode", gcnew NodeWidgetCreateRout(this, &BrainView::CreateCodeNodeWidget));
	for each (KeyValuePair<String ^, NodeWidgetCreateRout ^> ^p in createRouts) {
		MenuItem^ item = gcnew MenuItem();
		item->Header = "Create " + p->Key;
		item->Tag = p->Value;
		item->Click += gcnew System::Windows::RoutedEventHandler(this, &BrainView::NodesCreate);
		addNodeMenu_->Items->Add(item);
	}
}

void	BrainView::RunTree() {
	treeController_->Run(42);
}


BrainView::BrainView(void) :
	canvas_(gcnew Canvas()),
	scroll_(gcnew ScrollViewer()),
	fileMenu_(gcnew Menu()),
	dpWin_(gcnew DockPanel()),
	addNodeMenu_(gcnew System::Windows::Controls::ContextMenu()),
	consoleDebug_(gcnew TextBlock()),
	consoleDebugTitle_(gcnew TextBlock()),
	consoleGrid_(gcnew Grid()),
	treeController_(gcnew DecisionTreeController()) {
		this->Title = "PFA";
		this->Width = 1200;
		this->Height = 800;
		this->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x20, 0x20, 0x20));

		createNodesMenu_();
		this->scroll_->MouseRightButtonUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::RightClick);

		this->scroll_->MouseUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::OnMouseClickWheelUp);
		this->scroll_->MouseDown += gcnew System::Windows::Input::MouseButtonEventHandler(this, &BrainView::OnMouseClickWheelDown);
		this->scroll_->MouseMove += gcnew System::Windows::Input::MouseEventHandler(this, &BrainView::OnMouseMove);
		this->canvas_->Width = 10000;
		this->canvas_->Height = 10000;
		this->scroll_->HorizontalScrollBarVisibility = ScrollBarVisibility::Hidden;
		this->scroll_->VerticalScrollBarVisibility = ScrollBarVisibility::Hidden;
		this->scroll_->Content = this->canvas_;

		consoleDebugTitle_->Height = 25;
		consoleDebugTitle_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0xFF, 0x9F, 0x3F));
		consoleDebugTitle_->Text = "\tDebug Console";
		consoleDebugTitle_->Foreground = gcnew SolidColorBrush(Colors::White);
		//consoleDebug_->Height = 500;
		consoleDebug_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 100, 100, 100));
		consoleDebug_->Foreground = gcnew SolidColorBrush(Colors::White);
		consoleDebug_->TextWrapping = TextWrapping::WrapWithOverflow;
		ScrollViewer^ scrollConsole = gcnew ScrollViewer();
		scrollConsole->VerticalScrollBarVisibility = ScrollBarVisibility::Visible;
		scrollConsole->CanContentScroll = true;
		scrollConsole->Content = consoleDebug_;
		scrollConsole->ScrollToBottom();
		RowDefinition ^r = gcnew RowDefinition();
		r->Height = GridLength(25);
		RowDefinition ^r2 = gcnew RowDefinition();
		r2->Height = GridLength(180);
		consoleGrid_->RowDefinitions->Add(r);
		consoleGrid_->RowDefinitions->Add(r2);
		consoleGrid_->SetRow(consoleDebugTitle_, 0);
		consoleGrid_->SetRow(scrollConsole, 1);
		consoleGrid_->Height = 200;

		consoleGrid_->Children->Add(scrollConsole);
		consoleGrid_->Children->Add(consoleDebugTitle_);

		dpWin_->SetDock(fileMenu_, Dock::Top);
		dpWin_->SetDock(consoleGrid_, Dock::Bottom);
		dpWin_->SetDock(scroll_, Dock::Bottom);

		dpWin_->Children->Add(fileMenu_);
		dpWin_->Children->Add(consoleGrid_);
		dpWin_->Children->Add(scroll_);
		fileMenu_->IsMainMenu = true;
		MenuItem^ fileItem = gcnew MenuItem();
		fileItem->Header = "_File";
		fileMenu_->Items->Add(fileItem);
		MenuItem^ itemSave = gcnew MenuItem();
		itemSave->Header = "_Save";
		ImageSource ^imgSourceSave = gcnew BitmapImage(gcnew Uri(".\\save.jpg", UriKind::Relative));
		Image^ imgSave = gcnew Image();
		imgSave->Source = imgSourceSave;
		imgSave->Width = 20;
		imgSave->Height = 20;
		itemSave->Icon = imgSave;
		MenuItem^ itemLoad = gcnew MenuItem();
		itemLoad->Header = "_Load";
		BitmapImage ^btmLoad;
		ImageSource ^imgSourceLoad = gcnew BitmapImage(gcnew Uri(".\\save.jpg", UriKind::Relative));
		Image^ imgLoad = gcnew Image();
		imgLoad->Source = imgSourceLoad;
		imgLoad->Width = 20;
		imgLoad->Height = 20;
		itemLoad->Icon = imgLoad;
		fileItem->Items->Add(itemSave);
		fileItem->Items->Add(itemLoad);
		itemSave->Click += gcnew System::Windows::RoutedEventHandler(this, &BrainView::OnMouseClickSave);
		itemLoad->Click += gcnew System::Windows::RoutedEventHandler(this, &BrainView::OnMouseClickLoad);


		this->Content = dpWin_;
		this->SizeChanged += gcnew System::Windows::SizeChangedEventHandler(this, &BrainView::WinSizeChanged);
		//CodeNodeWidget ^a = gcnew CodeNodeWidget(this, 100, 10, "test1", gcnew CodeNode("int b;", consoleDebug_));
}

Canvas ^BrainView::getCanvas() {
	return canvas_;
}

void BrainView::OnMouseClickWheelDown(Object^ sender, MouseButtonEventArgs^ e)
{
	if (e->ChangedButton == MouseButton::Middle)
	{
		this->moveWheel_ = true;
		this->lastWheelX_ = e->GetPosition(this).X;
		this->lastWheelY_ = e->GetPosition(this).Y;
		this->Cursor = Cursors::Hand;
	}
}

void BrainView::OnMouseClickWheelUp(Object^ sender, MouseButtonEventArgs^ e)
{
	if (e->ChangedButton == MouseButton::Middle)
		this->moveWheel_ = false;
	this->Cursor = nullptr;
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

void BrainView::RightClick(Object^ sender, MouseButtonEventArgs^ e)
{
	this->addNodeMenu_->IsOpen = true;
}

NodeWidget<ANode ^>	^BrainView::CreateCodeNodeWidget() {
	CodeNode ^n = safe_cast<CodeNode ^>(treeController_->CreateCodeNode(consoleDebug_));
	gcnew CodeNodeWidget(this, 42 + this->scroll_->ContentHorizontalOffset, 42 + this->scroll_->ContentVerticalOffset, n->getName(), n);

	return nullptr;
}

void BrainView::NodesCreate(System::Object ^sender, RoutedEventArgs ^e)
{
	MenuItem ^i = (MenuItem ^)sender;
	NodeWidgetCreateRout ^nodeCreate = (NodeWidgetCreateRout ^)i->Tag; // to instantiate the right node

	nodeCreate->Invoke();
}

void BrainView::OnMouseClickWin(Object^ sender, MouseButtonEventArgs^ e)
{
	if (selected_) {
		if (mode_ == BrainView::Mode::MOVE) {
			FieldInfo ^fi = selected_->GetType()->GetField("recNode_");

			PropertyInfo ^strokeI = fi->FieldType->GetProperty("Stroke");

			strokeI->SetValue(fi->GetValue(selected_), gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF)), nullptr);
		}
		selected_ = nullptr;
		mode_ = BrainView::Mode::NONE;
	}
}

void BrainView::OnMouseClickSave(Object^ sender, RoutedEventArgs^ e)
{
	SaveFileDialog^ dialogSave = gcnew SaveFileDialog();

	dialogSave->Filter = "BrainFiles (*.xml) | *.xml";
	if (dialogSave->ShowDialog().Value) {
		treeController_->Save(dialogSave->FileName);
	}
}

void	BrainView::DrawCanvas() {
	Dictionary<ANode ^, Object ^>	widgets;
	array<NodeWidget<ANode ^> ^>	^widgetsCreate = gcnew array<NodeWidget<ANode ^> ^>(5);

	// Create widgets
	treeController_->tree_->setRootNode(nullptr);
	for each (ANode ^w in treeController_->getNodesList()) {
		Tuple<UInt32, UInt32>	^pos = w->getPosition();
		w->console_ = this->consoleDebug_;
		widgets.Add(w, gcnew CodeNodeWidget(this, pos->Item1, pos->Item2, w->getName(), (CodeNode ^)w));
	}
	/* We need to have all widgets before drawing the links */
	for each (KeyValuePair<ANode ^, Object ^> ^p in widgets) {
		for each (KeyValuePair<ANode ^, ANode ^> ^child in p->Key->getChildren()) {
			MethodInfo ^mi = widgets[child->Value]->GetType()->GetMethod("LinkChild");
			array<Object ^> ^args = gcnew array<Object ^>(1);
			args[0] = p->Value;
			mi->Invoke(widgets[child->Value], args);
		}
	}
	
	// for each node widget in win->widgets => create links
}

void BrainView::OnMouseClickLoad(Object^ sender, RoutedEventArgs^ e)
{
	OpenFileDialog^ dialogOpen = gcnew OpenFileDialog();

	dialogOpen->Filter = "BrainFiles (*.xml) | *.xml";
	if (dialogOpen->ShowDialog().Value)
	{
		treeController_->Load(dialogOpen->FileName);
		this->canvas_->Children->Clear();
		DrawCanvas();
	}
}

void BrainView::WinSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	dpWin_->Width = this->ActualWidth - 15;
	dpWin_->Height = this->ActualHeight - 40;
	consoleGrid_->Width = this->ActualWidth - 15;
}

