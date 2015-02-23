/*
** Author: Sebastien Maire
** Contribs:
*/

#include "CodeNode.hpp"
#include "OnTheFly\CompilerAttr.hpp"
#include "OnTheFly\NodeMainAttr.hpp"

using namespace System;
using namespace OnTheFly;

CodeNode::CodeNode(String ^code) :
code_(code), res_(nullptr), rebuild_(true), language_(LanguageSel::CSHARP) {
}

void CodeNode::Build() {
	/*
	array<CodeDomProvider ^> ^a = gcnew array<CodeDomProvider ^>(42);
	array<CompilerParameters ^> ^compilerParams = gcnew array<CompilerParameters ^>(42);

	Console::WriteLine("test");
	a[0] = CSharpCompilerAttr::provider;
	a[1] = JScriptCompilerAttr::provider;
	a[2] = VBCompilerAttr::provider;
	CompilerResults ^last = res_;
	compilerParams[0] = CSharpCompilerAttr::compilerParams;
	compilerParams[1] = JScriptCompilerAttr::compilerParams;
	compilerParams[2] = VBCompilerAttr::compilerParams;

	//CompilerParameters ^compilerParams = (CompilerParameters ^)(a[(int)language_]->GetField("compilerParams")->GetValue(a[(int)language_]));
	res_ = a[(int)language_]->CompileAssemblyFromSource(compilerParams[(int)language_], code_);
	Console::WriteLine("test2");

	//Invoke()->CompileAssemblyFromSource(CompilerConfig::compilerParams, code_);
	if (res_->Errors->Count != 0) {
	Console::WriteLine("Errors occured: getting back old code...");
	for each (CompilerError ^ce in res_->Errors) {
	Console::WriteLine(ce);
	}
	res_ = last;
	}
	*/
	CompilerResults ^last = res_;

	res_ = CompilerAttr::provider[(int)language_]->CompileAssemblyFromSource(CompilerAttr::compilerParams, code_);
	if (res_->Errors->Count != 0) {
		Console::WriteLine("Errors occured: getting back old code...");
		for each (CompilerError ^ce in res_->Errors) {
			Console::WriteLine(ce);
		}
		res_ = last;
	}
}

generic<typename T>
	void CodeNode::Process(T value) {
		Console::WriteLine("called with scalar");
		if (rebuild_) {
			this->Build();
			rebuild_ = false;
		}
		if (res_) {
			array<Type ^>	^types = res_->CompiledAssembly->GetTypes();
			if (types->Length <= 0) {
				Console::WriteLine("Error [G000]a");
			}
			Type ^entryClass = nullptr;
			for each (Type ^t in types) {
				if (entryClass && t->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
					Console::WriteLine("Error [G000]b");
				}
					Console::WriteLine(t->Name);
				if (t->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
					entryClass = t;
				}
			}
			if (!entryClass) {
				Console::WriteLine("Error [G000]c");
			}
			Object ^o = res_->CompiledAssembly->CreateInstance(entryClass->FullName);
			MethodInfo ^methodInfo = nullptr;
			for each (MethodInfo ^i in entryClass->GetMethods()) {
				if (methodInfo && i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
					Console::WriteLine("Error [G000]d");
				}
				if (i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
					methodInfo = i;
				}
			}
			if (!methodInfo) {
				Console::WriteLine("Error [G000]e");
			}
			//MethodInfo ^mi = o->GetType()->GetMethod("SayHello");
			methodInfo->Invoke(o, gcnew array < Object ^ > {childs_, value});
		}
	}
