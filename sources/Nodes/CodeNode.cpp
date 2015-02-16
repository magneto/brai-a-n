/*
** Author: Sebastien Maire
** Contribs:
*/

#include "CodeNode.hpp"

using namespace System;

generic<typename T>
CodeNode<T>::CodeNode(String ^code) :
	code_(code) {
}

generic<typename T>
void CodeNode<T>::Process(const T %value) {
	System::Console::Write("[");
	System::Console::Write(value);
	System::Console::Write("=>");
	System::Console::Write(this->code_);
	System::Console::WriteLine("]");
}