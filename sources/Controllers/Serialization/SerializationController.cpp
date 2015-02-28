/*
**	Author: Emmanuel Isidore
**	Contribs:Sébastien Maire
*/

#include "SerializationController.hpp"

Serializer::Serializer()
{formater_ = gcnew SoapFormatter;}


	void Serializer::Serialize(String^ filename, ANode^ obj)
	{
		Stream^ fileStream = File::Open(filename, FileMode::Create);
		formater_->Serialize(fileStream, obj);
		fileStream->Close();
	}

	ANode^ Serializer::Unserialize(String^ filename)
	{
		ANode^ obj;
		Stream^ fileStream = File::Open(filename, FileMode::Open);
		obj = dynamic_cast<ANode^>(formater_->Deserialize(fileStream));
		fileStream->Close();
		return obj;
	}