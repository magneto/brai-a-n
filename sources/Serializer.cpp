/*
**	Author: Emmanuel Isidore
**	Contribs:
*/

#include "Serializer.hpp"

generic<typename T>
Serializer<T>::Serializer()
{formater_ = gcnew SoapFormatter;}

generic<typename T>
	void Serializer<T>::Serialize(String^ filename, ANode<T>^ obj)
	{
		Stream^ fileStream = File::Open(filename, FileMode::Create);
		formater_->Serialize(fileStream, obj);
		fileStream->Close();
	}

generic<typename T>
	ANode<T>^ Serializer<T>::Unserialize(String^ filename)
	{
		ANode<T>^ obj;
		Stream^ fileStream = File::Open(filename, FileMode::Open);
		obj = dynamic_cast<ANode<T>^>(formater_->Deserialize(fileStream));
		fileStream->Close();
		return obj;
	}