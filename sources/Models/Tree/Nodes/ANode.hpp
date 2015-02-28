/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#using <System.dll>

using namespace System;
using namespace System::Collections::Generic;

ref class Counter {
public:
	static int last = 0;
	static LinkedList<int> ^avnames = gcnew LinkedList<int>();
	static int getNumber() {
		int num;
		if (avnames->Count > 0) {
			num = Counter::avnames->First->Value;
			avnames->RemoveFirst();
		}
		else
		{
			num = last++;
		}
		return num;
	}
	static void removeNumber(int n) {
		Counter::avnames->Remove(n);
	}
};

public ref class ANode abstract {
protected:
	Dictionary<String^, ANode^>	^children_;
	UInt32	posX_;
	UInt32	posY_;
public:
	int number;
	String ^name_;
	ANode();
// Getters, setters
	void			setPosition(UInt32 x, UInt32 y);
	String	^getName();
	Dictionary<String ^, ANode ^>	^getChildren();

	void AddChild(ANode ^node);
	generic<typename T>
	virtual void Process(T val) abstract;
};