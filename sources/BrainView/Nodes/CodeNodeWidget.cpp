/*
** Author: Stephane Nguyen
** Contribs: Sebastien Maire
*/

#include "CodeNodeWidget.hpp"
#include "Models\Tree\Nodes\CodeNode.hpp"
#include "BrainView\BrainView.hpp"

using namespace System::Windows::Media::Imaging;

CodeNodeWidget::CodeNodeWidget(BrainView ^curWin, int posX, int posY, String ^title, CodeNode ^node) :
	NodeWidget<CodeNode ^>(curWin, posX, posY, title, node),
	tBox_(gcnew TextBox())
 {
		tBox_->Width = 350;
		tBox_->Height = 220;
		tBox_->Background = gcnew SolidColorBrush(Color::FromArgb(0xFF, 100, 100, 100));
		tBox_->BorderBrush = gcnew SolidColorBrush(Color::FromArgb(0xFF, 0x26, 0xBE, 0xEF));
		tBox_->BorderThickness = System::Windows::Thickness(0, 1, 0, 0);
		tBox_->Margin = System::Windows::Thickness(0, -1, 0, 0);
		tBox_->Foreground = gcnew SolidColorBrush(Colors::White);
		tBox_->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
		tBox_->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
		tBox_->TextChanged += gcnew System::Windows::Controls::TextChangedEventHandler(this, &CodeNodeWidget::tBoxChanged);
		tBox_->AcceptsReturn = true;
		tBox_->Text = node_->code_;
		spRoot_->Children->Add(tBox_);

}

void CodeNodeWidget::tBoxChanged(Object ^sender, TextChangedEventArgs ^e) {
	node_->code_ = tBox_->Text;
}