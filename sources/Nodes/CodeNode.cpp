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
ANode(), code_(code), res_(nullptr), rebuild_(true), language_(LanguageSel::CSHARP) {
}

void CodeNode::Build() {
	CompilerResults ^last = res_;

	res_ = CompilerAttr::provider[(int)language_]->CompileAssemblyFromSource(CompilerAttr::compilerParams, code_);
	if (res_->Errors->Count != 0) {
		TextWriter ^errorWriter = Console::Error;

		errorWriter->WriteLine("ERROR: (0x0)=>Build failed on node \"{0}\" with following errors:", name_);
		for each (CompilerError ^ce in res_->Errors) {
			errorWriter->WriteLine(ce);
		}
		res_ = last;
	}
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
	instance_ = res_->CompiledAssembly->CreateInstance(entryClass->FullName);
	entryPoint_ = nullptr;
	for each (MethodInfo ^i in entryClass->GetMethods()) {
		if (entryPoint_ && i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			Console::WriteLine("Error [G000]d");
		}
		if (i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			entryPoint_ = i;
		}
	}
	if (!entryPoint_) {
		Console::WriteLine("Error [G000]e");
	}
}

generic<typename T>
	void CodeNode::Process(T value) {
		if (rebuild_) {
			this->Build();
			rebuild_ = false;
		}
		if (res_) {
			//Console::WriteLine(name_);

			entryPoint_->Invoke(instance_, gcnew array < Object ^ > {childs_, value});
		}
	}
