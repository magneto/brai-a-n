/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#include "ANode.hpp"

#using "system.dll"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Net;
using namespace Microsoft::CSharp;
using namespace System::CodeDom::Compiler;


private ref class CompilerConfig {
public:
	static CompilerConfig() {
		providerOptions->Add("CompilerVersion", "v3.5");
		compilerParams->GenerateInMemory = true;
		compilerParams->GenerateExecutable = false;
		provider = gcnew CSharpCodeProvider(providerOptions);
	//CodeDomProvider ^a = provider;
	}
	static CSharpCodeProvider ^provider;
	static CompilerParameters ^compilerParams = gcnew CompilerParameters();
	static Dictionary<String^, String^> ^providerOptions = gcnew Dictionary<String^, String^>();
};

generic<typename T>
public ref class CodeNode : public ANode<T> {
protected:
	String ^code_;
	CompilerResults ^res_;
public:
	CodeNode(String ^code);
	void Build();
	void Process(T %value) override;
	void Process(T value) override;
};