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
			StringBuilder ^s = (gcnew StringBuilder())->Append("OnTheFly")->Append("42.dll");
			compilerParams->GenerateInMemory = true;
			compilerParams->GenerateExecutable = false;
			//compilerParams->OutputAssembly = s->ToString();
			compilerParams->ReferencedAssemblies->Add("System.dll");
			//compilerParams->ReferencedAssemblies->Add("System.ComponentModel.Composition.dll");
			compilerParams->ReferencedAssemblies->Add((gcnew System::Uri(Assembly::GetExecutingAssembly()->CodeBase))->AbsolutePath);
			provider = gcnew array < CodeDomProvider ^ > {
				CodeDomProvider::CreateProvider(languages[0], providerOptions),
				CodeDomProvider::CreateProvider(languages[1]),
				CodeDomProvider::CreateProvider(languages[2], providerOptions)
			};
		}
		static array<CodeDomProvider ^>	^provider;
		static CompilerParameters ^compilerParams = gcnew CompilerParameters();
		static Dictionary<String^, String^> ^providerOptions = gcnew Dictionary<String^, String^>();
	};
}
