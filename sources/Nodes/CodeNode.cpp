/*
** Author: Sebastien Maire
** Contribs:
*/

#include "CodeNode.hpp"

using namespace System;

generic<typename T>
CodeNode<T>::CodeNode(String ^code) :
	code_(code), res_(nullptr) {
}

generic<typename T>
void CodeNode<T>::Build() {
	CompilerResults ^last = res_;

	res_ = CompilerConfig::provider->CompileAssemblyFromSource(CompilerConfig::compilerParams, code_);
	if (res_->Errors->Count != 0) {
		Console::WriteLine("Errors occured: getting back old code...");
		for each (CompilerError ^ce in res_->Errors) {
			Console::WriteLine(ce);
		}
		res_ = last;
	}
}

generic<typename T>
void CodeNode<T>::Process(T %value) {
	Console::WriteLine("Called with ref");
	if (res_) {
		Object ^o = res_->CompiledAssembly->CreateInstance("Node");

		MethodInfo ^mi = o->GetType()->GetMethod("Input");
		array<Object ^> ^a = gcnew array<Tuple<T> ^>(2);
		a[0] = Tuple::Create(value);
		mi->Invoke(o, a);
	}
}

generic<typename T>
void CodeNode<T>::Process(T value) {
	Console::WriteLine("called with scalar");
	if (res_) {
		Object ^o = res_->CompiledAssembly->CreateInstance("Node");

		MethodInfo ^mi = o->GetType()->GetMethod("Input");
		array<Object ^> ^a = gcnew array<Tuple<T> ^>(2);
		a[0] = Tuple::Create(value);
		mi->Invoke(o, a);
	}
}
