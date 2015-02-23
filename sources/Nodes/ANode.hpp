/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

using namespace System;
using namespace System::Collections::Generic;

//ref class ANode;

public ref class ANode abstract {
protected:
	Dictionary<String^, ANode^>	^childs_;
public:
	void AddChild(String ^name, ANode ^node);
	generic<typename T>
	virtual void Process(T val) abstract;
};