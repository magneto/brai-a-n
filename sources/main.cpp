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
	static BrainView^ win = gcnew BrainView();

	Appli(void){}
	static void	ThreadRout() {
		Console::WriteLine("toto");
		win->RunTree();
	}
protected:
	virtual void OnStartup(StartupEventArgs^ e) override
	{
		Application::OnStartup(e);

		System::Threading::Thread ^t = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(ThreadRout));
		t->Start();
		win->Show();
	}
};

[STAThread]
int main(array<String^>^ args)
{

	return (gcnew Appli())->Run();
}