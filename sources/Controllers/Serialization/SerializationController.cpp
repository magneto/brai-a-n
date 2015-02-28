/*
**	Author: Emmanuel Isidore
**	Contribs:Sébastien Maire
*/

#include "SerializationController.hpp"

Serializer::Serializer()
{formater_ = gcnew SoapFormatter;}


	void Serializer::Serialize(String^ filename, DecisionTree^ obj)
	{
		Stream^ fileStream = File::Open(filename, FileMode::Create);
		formater_->Serialize(fileStream, obj);
		fileStream->Close();
	}

	DecisionTree^ Serializer::Unserialize(String^ filename)
	{
		DecisionTree^ obj;
		Stream^ fileStream = File::Open(filename, FileMode::Open);
		obj = dynamic_cast<DecisionTree^>(formater_->Deserialize(fileStream));
		fileStream->Close();
		return obj;
	}