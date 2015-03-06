/*
** Author: Sebastien Maire
** Contribs: Stephane Nguyen
*/

#using "C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.5\PresentationFramework.dll"

#include "BrainView\BrainView.hpp"

using namespace System;
using namespace System::Windows;



ref class Appli : Application
{
public:
	Appli(void){}
protected:
	virtual void OnStartup(StartupEventArgs^ e) override
	{
		Application::OnStartup(e);
		BrainView^ win = gcnew BrainView();
		win->Show();
		// System::Threading::Thread ^t = gcnew System::Threading::Thread(BrainView::RunTree);
	}
};

[STAThread]
int main(array<String^>^ args)
{

	return (gcnew Appli())->Run();
}