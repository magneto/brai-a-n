/*
** Author: Emmanuel Isidore
** Contribs: Sébastien Maire
*/

#pragma once

#using <system.dll>
#using <system.messaging.dll>

#include "Models\Tree\DecisionTree.hpp"
#using <System.Xml.dll>
using namespace System::Xml::Serialization;
using namespace System::Runtime::Serialization::Formatters::Binary;


using namespace System;
using namespace System::IO;

public ref class Serializer
{
private:
	BinaryFormatter^ formater_;

public:
	Serializer();
	void Serialize(String^ , DecisionTree^% );
	DecisionTree^ Unserialize(String^ const);
};