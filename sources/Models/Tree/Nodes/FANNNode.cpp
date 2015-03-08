/*
** Author: Emmanuel Isidore
** Contribs:
*/


#include "FANNNode.hpp"

FANNNode::FANNNode():
outnb_(0), generated_(false), ann_(nullptr), output_(nullptr)
{}

FANNNode::~FANNNode()
{
	if (ann_ != nullptr)
		fann_destroy(this->ann_);
}

void FANNNode::build()
{
	ann_ = fann_create_standard(3, 1, 3, children_->Count);
	this->outnb_ = children_->Count;
}

generic<typename T>
void FANNNode::Process(T value)
{
	int i = 0;
	if (children_->Count == 0)
		return;
	if ((generated_ == false) || (outnb_ != children_->Count))
		build();
	*input_father_ = (fann_type)(value);
	output_ = (ann_, input_father_);
	for each (KeyValuePair<String^, ANode^> entry in children_)
	{
		if (output_[i] < 1)
			++i;
		else
		{
			entry.Value->Process<T>((T)output_[i]);
			break;
		}
	}
}