/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#using <System.dll>
#using <Microsoft.JScript.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Net;
using namespace System::CodeDom::Compiler;
using namespace Microsoft::CSharp;
using namespace Microsoft::JScript;
using namespace Microsoft::VisualBasic;

namespace OnTheFly {
	ref class CompilerAttr {

	public:
		static CompilerAttr() {
			array<String ^> ^languages = gcnew array < String ^ > {
				"CSharp",
				"JScript",
				"VisualBasic"
			};

			providerOptions->Add("CompilerVersion", "v4.0");
			compilerParams->GenerateInMemory = true;
			compilerParams->GenerateExecutable = false;

			// Add references to nodes
			compilerParams->ReferencedAssemblies->Add("System.dll");
			compilerParams->ReferencedAssemblies->Add("C:\\Program Files (x86)\\Reference Assemblies\\Microsoft\\Framework\\.NETFramework\\v4.5\\PresentationCore.dll");
			compilerParams->ReferencedAssemblies->Add("C:\\Program Files (x86)\\Reference Assemblies\\Microsoft\\Framework\\.NETFramework\\v4.5\\System.Xaml.dll");
			compilerParams->ReferencedAssemblies->Add("C:\\Program Files (x86)\\Reference Assemblies\\Microsoft\\Framework\\.NETFramework\\v4.5\\WindowsBase.dll");
			compilerParams->ReferencedAssemblies->Add("C:\\Program Files (x86)\\Reference Assemblies\\Microsoft\\Framework\\.NETFramework\\v4.5\\PresentationFramework.dll");
			compilerParams->ReferencedAssemblies->Add((gcnew System::Uri(Assembly::GetExecutingAssembly()->CodeBase))->AbsolutePath);

			provider = gcnew array < CodeDomProvider ^ > {
				CodeDomProvider::CreateProvider(languages[0], providerOptions),
					CodeDomProvider::CreateProvider(languages[1]),
					CodeDomProvider::CreateProvider(languages[2], providerOptions)
			};
		}
		static array<CodeDomProvider ^>		^provider;
		static CompilerParameters			^compilerParams = gcnew CompilerParameters();
		static Dictionary<String^, String^> ^providerOptions = gcnew Dictionary<String^, String^>();
	};
}
