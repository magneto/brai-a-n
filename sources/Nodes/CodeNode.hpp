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

generic<typename T>
public ref class CodeNode : public ANode<T> {
protected:
	String ^code_;
	
public:
	CodeNode(String ^code);
	void Process(const T %value) override;
};