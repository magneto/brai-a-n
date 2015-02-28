/*
** Author: Emmanuel Isidore
** Contribs: Sébastien Maire
*/

#pragma once

#using <system.dll>
#using <system.messaging.dll>
#using <System.Runtime.Serialization.Formatters.Soap.dll>

#include "Models\Tree\Nodes\ANode.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::Serialization::Formatters::Soap;

public ref class Serializer
{
private:
	SoapFormatter^ formater_;

public:
	Serializer();
	void Serialize(String^ , ANode^ );
	ANode^ Unserialize(String^ const);
};