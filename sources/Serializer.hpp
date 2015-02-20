/*
** Author: Emmanuel Isidore
** Contribs: 
*/

#pragma once
#using <system.dll>
#using <system.messaging.dll>
#using <System.Runtime.Serialization.Formatters.Soap.dll>
#include "Nodes\ANode.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::Serialization::Formatters::Soap;

generic<typename T>
public ref class Serializer
{
private:
	SoapFormatter^ formater_;

public:
	Serializer();
	void Serialize(String^ , ANode<T>^ );
	ANode<T>^ Unserialize(String^ const);
};